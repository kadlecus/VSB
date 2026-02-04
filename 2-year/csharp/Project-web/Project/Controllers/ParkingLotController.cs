using Microsoft.AspNetCore.Mvc;
using Project.Models;

public class ParkingLotController : Controller
{
    private readonly Database _database;
    public ParkingLotController(Database database)
    {
        _database = database;
    }

    public async Task<IActionResult> Index()
    {
       var pl = await _database.GetAllParkingLotsAsync();
       await _database.SaveOldReservations();
        foreach (var item in pl)
        {
            
            item.FreeSpots = await _database.CountFreeSpotsAsync(item.Id);
        }
     
        ViewBag.ParkingLots = pl;
        return View();
    }

    public async Task<IActionResult> Detail(int id)
    {
        await _database.SaveOldReservations(); 
        ViewBag.ParkingLotId = id;
        ViewBag.ParkingSpots = await _database.GetParkingSpotsWithStatusAsync(id);
        ViewBag.AdminSpots = await _database.GetParkingSpotsAsync(id);
        return View();
    }

    public IActionResult CreateReservation(int id)
    {
        ViewBag.ParkingLotId = id;


        if (HttpContext.Session.GetString("Role") != "Registered" || HttpContext.Session.GetString("Role") == "Admin")
        {
            return RedirectToAction("Login", "Account");
        }

        return View();
    }

    [HttpPost]
    public async Task<IActionResult> Reservation(int parkingLotId, DateTime startTime, DateTime endTime, string licensePlate)
    {
        if (endTime < startTime || startTime < DateTime.Now || endTime < DateTime.Now)
        {
            return View(new ReservationViewModel { Message = "Špatně zadaný čas" });
        }

        string? userEmail = HttpContext.Session.GetString("Email");
        int count = await _database.CountActiveReservations(userEmail);

        if (count >= 1)
        {
            return View(new ReservationViewModel { Message = $"Dosáhl jste maximálního počtu rezervací {count}" });
        }

        ParkingReservation? success = await _database.CreateReservationAsync(parkingLotId, startTime, endTime, licensePlate, userEmail);

        if (success != null)
        {
            return View(new ReservationViewModel
            {
                Message = "Rezervace byla úspěšně vytvořena",
                Reservation = success
            });
        }
        else
        {
            return View(new ReservationViewModel { Message = "Žádné volné místo" });
        }
    }


    [HttpPost]
    public async Task<IActionResult> EditStatus(string status, int spotid, string newStatus)
    {
        if (HttpContext.Session.GetString("Role") != "Admin")
        {
            return Unauthorized();
        }

        if (status == "Obsazené")
        {
            TempData["Error"] = "Místo je právě rezervované";
            int id = await _database.GetIdOfParkingLotAsync(spotid);
            return RedirectToAction("Detail", "ParkingLot", new { id = id });
        }

        try
        {
            await _database.EditStatusAsync(spotid, newStatus);
            TempData["Success"] = "Stav byl úspěšně změněn.";
        }
        catch (Exception ex)
        {
            TempData["Error"] = $"Chyba při změně stavu: {ex.Message}";
        }

        int parkingLotID = await _database.GetIdOfParkingLotAsync(spotid);
        return RedirectToAction("Detail", "ParkingLot", new { id = parkingLotID });
    }

    public async Task<IActionResult> Statistic()
    {

        ViewBag.Stats = await _database.GetParkingStatisticsAsync();
    
        return View ();
    }

    [HttpPost]
    public async Task<IActionResult> AddParkingSpot(int lotid)
    {
        if (HttpContext.Session.GetString("Role") != "Admin")
        {
            return Unauthorized();
        }

        if (await _database.AddParkingSpot(lotid))
        {
            TempData["SuccessAdd"] = "Parkovací místo bylo úspěšně přidáno.";
        }
        else
        {
            TempData["SuccessAdd"] = "Nepodařilo se přidat parkovací místo.";
        }

        return RedirectToAction("Detail", "ParkingLot", new { id = lotid });
    }

    [HttpPost]
    public async Task<IActionResult> DeleteSpot(int spotId)
    {
        if (HttpContext.Session.GetString("Role") != "Admin")
        {
            return Unauthorized();
        }

        int lotid = await _database.GetIdOfParkingLotAsync(spotId);
        if ( await _database.DeleteParkingSpot(spotId))
        {
            TempData["SuccessAdd"] = "Parkovací místo bylo úspěšně odstraněno.";
        }
        else
        {
            TempData["SuccessAdd"] = "Nepodařilo se odstranit parkovací místo.";
        }
        return RedirectToAction("Detail", "ParkingLot", new { id = lotid });
    }


    [HttpPost]
    public async Task<IActionResult> SpotHistory(int spotid)
    {
        ViewBag.SpotHistory = await _database.GetHistoryOfParkingSpotAsync(spotid);

        return View();
    }

}
