using System.Data;
using System.Threading.Tasks;
using Microsoft.Data.Sqlite;
using Dapper;
using System.Collections.Generic;
using Project.Models;
using static Microsoft.EntityFrameworkCore.DbLoggerCategory.Database;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion.Internal;
using System.ComponentModel;
using Microsoft.AspNetCore.Mvc;
using Microsoft.VisualBasic;
using Microsoft.AspNetCore.Mvc.Filters;



public class Database
{
    private readonly string _connectionString;

    public Database(string connectionString)
    {
        _connectionString = connectionString;
    }

    public async Task<SqliteConnection> CreateConnectionAsync()
    {
        SqliteConnection connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();
        return connection;
    }

    public async Task InitializeDatabaseAsync(string sqlFilePath)
    {
        string sqlScript = await File.ReadAllTextAsync(sqlFilePath);
        Console.WriteLine(sqlScript);
        using SqliteConnection connection = await CreateConnectionAsync();
        await connection.ExecuteAsync(sqlScript);
    }

    public async Task<IEnumerable<ParkingLot>> GetAllParkingLotsAsync()
    {
        using SqliteConnection conn = await CreateConnectionAsync();
        string sql = "SELECT * FROM ParkingLots";
        return await conn.QueryAsync<ParkingLot>(sql);

    }

    public async Task<int> GetIdOfParkingLotAsync(int spotId)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"SELECT ParkingLotId FROM ParkingSpots WHERE Id = @id";

        return await conn.ExecuteScalarAsync<int>(sql,new { id = spotId});

    }

    public async Task<IEnumerable<ParkingSpot>> GetParkingSpotsAsync(int parkingLotId) 
    {
        using SqliteConnection conn = await CreateConnectionAsync();
        string sql = @"
        SELECT ps.Id, ps.ManualStatus,
               CASE 
                   WHEN EXISTS (
                       SELECT 1 
                       FROM ParkingReservation pr
                       WHERE pr.ParkingSpotId = ps.Id 
                         AND pr.StartTime <= @now 
                         AND pr.EndTime > @now
                   )
                   THEN 'Obsazené'
                   ELSE 'Volné'
               END AS Status
        FROM ParkingSpots ps
        WHERE ps.ParkingLotId = @parkingLotId;";

        var result = await conn.QueryAsync<ParkingSpot>(sql, new { parkingLotId, now = DateTime.Now });
        return result;
    }
    public async Task<IEnumerable<ParkingSpot>> GetParkingSpotsWithStatusAsync(int parkingLotId)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"
        SELECT ps.Id, ps.ManualStatus,
               CASE 
                   WHEN EXISTS (
                       SELECT 1 
                       FROM ParkingReservation pr
                       WHERE pr.ParkingSpotId = ps.Id 
                         AND pr.StartTime <= @now 
                         AND pr.EndTime > @now
                   )
                   THEN 'Obsazené'
                   ELSE 'Volné'
               END AS Status
        FROM ParkingSpots ps
        WHERE ps.ParkingLotId = @parkingLotId
          AND (ps.ManualStatus IS NULL OR ps.ManualStatus != 'V údržbě');";

        var result = await conn.QueryAsync<ParkingSpot>(sql, new { parkingLotId, now = DateTime.Now });
        return result;
    }

    public async Task<ParkingSpot?> GetRandomAvailableParkingSpotAsync(int parkingLotId, DateTime fromDate, DateTime toDate)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"
    SELECT * FROM ParkingSpots
    WHERE ParkingLotId = @parkingLotId
      AND ManualStatus != 'V údržbě'
      AND Id NOT IN (
          SELECT pr.ParkingSpotId
          FROM ParkingReservation pr
          WHERE pr.EndTime > @fromDate AND pr.StartTime < @toDate
      );";

        var availableSpots = (await conn.QueryAsync<ParkingSpot>(sql, new { parkingLotId, fromDate, toDate })).ToList();

        if (!availableSpots.Any())
            return null;

        Random rnd = new Random();
        return availableSpots[rnd.Next(availableSpots.Count)];
    }

    public async Task<ParkingReservation?> CreateReservationAsync(int parkingLotId, DateTime fromDate, DateTime toDate, string licensePlate, string UserEmail)
    {
        if (toDate <= fromDate)
        {
            return null; 
        
        }

        using SqliteConnection conn = await CreateConnectionAsync();

       
        ParkingSpot? selectedSpot = await GetRandomAvailableParkingSpotAsync(parkingLotId, fromDate, toDate);

        if (selectedSpot == null)
        {
            
            return null;
        }
       
        var reservation = new ParkingReservation
        {
            ParkingLotId = parkingLotId,
            ParkingSpotId = selectedSpot.Id,
            LicensePlate = licensePlate,
            StartTime = fromDate,
            EndTime = toDate,
            Duration = (int)(toDate - fromDate).TotalMinutes,
            UserEmail = UserEmail
        };

        reservation.Price = reservation.CalculatePriceForDuration();



        string insertSql = @"
        INSERT INTO ParkingReservation (ParkingLotId, ParkingSpotId, LicensePlate, StartTime, EndTime, Duration, Price, UserEmail)
        VALUES (@ParkingLotId, @ParkingSpotId, @LicensePlate, @StartTime, @EndTime, @Duration, @Price, @UserEmail);";

        await conn.ExecuteAsync(insertSql, reservation);
        return reservation;
    }

    public async Task CreateUserAsync(User user)
    {
        using SqliteConnection conn = await CreateConnectionAsync();
        string sql = "INSERT INTO Users (Username, Email, PasswordHash, Role) VALUES (@Username, @Email, @PasswordHash, @Role)";
        await conn.ExecuteAsync(sql, user);
    }

    public async Task<User?> GetUserByEmailOrUsername(string UsernameOrEmail)
    {
        using SqliteConnection conn = await CreateConnectionAsync();
        string sql = "SELECT * FROM USERS WHERE Username = @usernameOremail OR Email = @usernameOremail";

        return await conn.QueryFirstOrDefaultAsync<User>(sql, new { usernameOremail = UsernameOrEmail });
    }

    public async Task<ParkingSpot?> GetParkingSpotByIdAsync(int spotid)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = "SELECT * FROM ParkingSpots WHERE Id = @id";

        return await conn.QueryFirstOrDefaultAsync<ParkingSpot>(sql, new { id = spotid });
    }

    public async Task<ParkingReservation?> GetReservationByIdAsync(int reservationId)
    {
        using SqliteConnection conn = await CreateConnectionAsync();
        string sql = "SELECT * FROM ParkingReservation WHERE Id = @id";

        return await conn.QueryFirstOrDefaultAsync<ParkingReservation>(sql, new { id = reservationId });
    }

    public async Task EditStatusAsync(int spotid, string newStatus)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = "UPDATE ParkingSpots SET ManualSTATUS = @status WHERE Id = @id";

        int rowsAffected = await conn.ExecuteAsync(sql, new { id = spotid, status = newStatus });

        if (rowsAffected == 0)
        {
            throw new Exception("Nic nebylo aktualizovano");
        }   

    }

    public async Task SaveOldReservations()
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string insertSql = @"
        INSERT INTO ParkingReservationHistory (ParkingLotId, ParkingSpotId, LicensePlate, StartTime, EndTime, Duration, Price, UserEmail)
        SELECT ParkingLotId, ParkingSpotId, LicensePlate, StartTime, EndTime, Duration, Price, UserEmail
        FROM ParkingReservation
        WHERE EndTime <= @now;";

        string deleteSql = @"
        DELETE FROM ParkingReservation
        WHERE EndTime <= @now;";

        using var transaction = conn.BeginTransaction();

        await conn.ExecuteAsync(insertSql, new { now = DateTime.Now }, transaction);
        await conn.ExecuteAsync(deleteSql, new { now = DateTime.Now }, transaction);

        await transaction.CommitAsync();
    }
    

    public async Task<int> CountFreeSpotsAsync(int lotId)
    {

        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"
        SELECT COUNT(ps.Id)
        FROM ParkingSpots ps
        WHERE ps.ParkingLotId = @id
          AND ps.ManualStatus != 'V údržbě'
          AND NOT EXISTS (
              SELECT 1
              FROM ParkingReservation pr
              WHERE pr.ParkingSpotId = ps.Id
                AND pr.StartTime <= @now
                AND pr.EndTime > @now
          )";

        return await conn.ExecuteScalarAsync<int>(sql, new { id = lotId , now = DateTime.Now});

    }

    public async Task<IEnumerable<ParkingReservation>> GetAllReservationsAsync()
    {
        SqliteConnection conn = await CreateConnectionAsync();

        string sql = "SELECT * FROM ParkingReservation";

        var reservations = await conn.QueryAsync<ParkingReservation>(sql);

        return reservations;
    }


    public async Task<IEnumerable<ParkingReservation?>> GetParkingReservationAsync(string? Email)
    {
        if (Email == null)
        {
            return Enumerable.Empty<ParkingReservation>();
        }
        using SqliteConnection conn = await CreateConnectionAsync();


        string sql = @"SELECT pr.*, pl.Name AS ParkingLotName
                    FROM ParkingReservation pr
                    JOIN ParkingSpots ps ON pr.ParkingSpotId = ps.Id
                    JOIN ParkingLots pl ON ps.ParkingLotId = pl.Id
                    WHERE pr.UserEmail = @email";

        return await conn.QueryAsync<ParkingReservation?>(sql, new { email = Email });
        
    }

    public async Task<IEnumerable<ParkingReservation?>> GetParkingReservationHistoryAsync(string? Email)
    {
        if (Email == null)
        {
            return Enumerable.Empty<ParkingReservation>();
        }
        using SqliteConnection conn = await CreateConnectionAsync();


        string sql = @"SELECT pr.*, pl.Name AS ParkingLotName
                    FROM ParkingReservationHistory pr
                    JOIN ParkingSpots ps ON pr.ParkingSpotId = ps.Id
                    JOIN ParkingLots pl ON ps.ParkingLotId = pl.Id
                    WHERE pr.UserEmail = @email";

        return await conn.QueryAsync<ParkingReservation?>(sql, new { email = Email });

    }

    public async Task<IEnumerable<ParkingReservation?>> GetAllParkingReservationHistoryAsync()
    {
      
        using SqliteConnection conn = await CreateConnectionAsync();


        string sql = @"SELECT pr.*, pl.Name AS ParkingLotName
                    FROM ParkingReservationHistory pr
                    JOIN ParkingSpots ps ON pr.ParkingSpotId = ps.Id
                    JOIN ParkingLots pl ON ps.ParkingLotId = pl.Id
                    ";

        return await conn.QueryAsync<ParkingReservation?>(sql);

    }




    public async Task DeleteReservationAsync(int reservationId)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string deleteSql = @"DELETE FROM ParkingReservation WHERE Id = @id";
        ParkingReservation? reservation = await GetReservationByIdAsync(reservationId);

        if (reservation == null)
            throw new InvalidOperationException("Rezervace nebyla nalezena.");


        if (reservation.StartTime > DateTime.Now)
        {
            await conn.ExecuteAsync(deleteSql, new { id = reservationId });
        }
        else
        {
            reservation.EndTime = DateTime.Now;
            await UpdateReservationAsync(reservation);

            await SaveOldReservations();
        }   
    }

    public async Task UpdateReservationAsync(ParkingReservation reservation)
    {
        if(reservation.StartTime > reservation.EndTime)
        {
            throw new InvalidOperationException("Špatně zadaný čas. Nic nebylo změněno");
 
        }
        

        using SqliteConnection conn = await CreateConnectionAsync();

        

        reservation.Duration = (int)(reservation.EndTime - reservation.StartTime).TotalMinutes;
        decimal price = reservation.CalculatePriceForDuration();
        reservation.Price = price;

        string sql = @"UPDATE ParkingReservation 
                   SET
                       StartTime = @StartTime,
                       EndTime = @EndTime,
                       Duration = @Duration,
                       Price = @Price
                   WHERE Id = @Id";


        await conn.ExecuteAsync(sql, reservation);
    }

    public async Task<IEnumerable<EndedReservationsCount>> GetParkingStatisticsAsync()
    {

        using SqliteConnection connection = await CreateConnectionAsync();

        string sql = @"
       SELECT ParkingLotId, COUNT(*) AS Completed
        FROM ParkingReservationHistory
        WHERE EndTime >= DATETIME('now', '-1 month')
          AND EndTime <= DATETIME('now')
        GROUP BY ParkingLotId";

        
        return await connection.QueryAsync<EndedReservationsCount>(sql);
        
    }

    public async Task<int> CountActiveReservations(string userEmail)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"
        SELECT COUNT(*) 
        FROM ParkingReservation 
        WHERE UserEmail = @email";

        var count = await conn.ExecuteScalarAsync<int?>(sql, new { email = userEmail });

        return count ?? 0;
    }

    public async Task<bool> AddParkingSpot(int lotid)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"INSERT INTO ParkingSpots (ParkingLotId, ManualStatus) VALUES (@lotid, 'Dostupné')";

        int rowsaffected = await conn.ExecuteAsync(sql, new { lotid = lotid });

        return rowsaffected > 0;
    }

    public async Task<bool> DeleteParkingSpot(int spotid)
    {
        using SqliteConnection conn = await CreateConnectionAsync();
        await conn.OpenAsync();

        using var transaction = await conn.BeginTransactionAsync();

        try
        {
            string sqlDeleteSpotInStatus = @"DELETE FROM ParkingSpotStatusHistory WHERE ParkingSpotId = @id";
            string sqlDeleteSpotInHistory = @"DELETE FROM ParkingReservationHistory WHERE ParkingSpotId = @id";
            string sqlDeleteSpotInReservation = @"DELETE FROM ParkingReservation WHERE ParkingSpotId = @id";
            string sqlDeleteSpot = @"DELETE FROM ParkingSpots WHERE Id = @id";


            await conn.ExecuteAsync(sqlDeleteSpotInStatus, new { id = spotid }, transaction);
            await conn.ExecuteAsync(sqlDeleteSpotInHistory, new { id = spotid }, transaction);
            await conn.ExecuteAsync(sqlDeleteSpotInReservation, new { id = spotid }, transaction);
            int rowsaffected = await conn.ExecuteAsync(sqlDeleteSpot, new { id = spotid }, transaction);

            

            await transaction.CommitAsync();

            return rowsaffected > 0; 
        }
        catch (Exception ex)
        {
            await transaction.RollbackAsync();
            throw new Exception("Chyba při mazání parkovacího místa.", ex);

        }
    }

    public async Task<IEnumerable<ParkingStatusHistory?>> GetHistoryOfParkingSpotAsync(int spotid)
    {
        using SqliteConnection conn = await CreateConnectionAsync();

        string sql = @"SELECT * FROM ParkingSpotStatusHistory WHERE ParkingSpotId = @id";

        return await conn.QueryAsync<ParkingStatusHistory?>(sql,new { id = spotid});
    }






}
