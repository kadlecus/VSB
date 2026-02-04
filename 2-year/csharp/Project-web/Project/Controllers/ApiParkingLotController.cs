using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Routing;
using Project.Models;
using System.Net.Http.Headers;
using System.Reflection;
namespace Project.Controllers
{
    [ApiController]
    [Route("ApiParkingLot")]
    public class ApiParkingLotController : Controller
    {
        private readonly Database _database;

        public ApiParkingLotController(Database database)
        {
            _database = database;
        }

        [HttpGet]
        public async Task<IActionResult> GetAllParkingLots()
        {
            var lots = await _database.GetAllParkingLotsAsync();

            foreach (var item in lots)
            {
                item.FreeSpots = await _database.CountFreeSpotsAsync(item.Id);
            }

            return Ok(lots);
        }

        [HttpGet("Detail/{lotid}")]
        public async Task<IActionResult> GetParkingLot(int lotid)
        {
            var spots = await _database.GetParkingSpotsAsync(lotid);
            return Ok(spots);
        }



        [HttpPost("Reservation")]
        public async Task<IActionResult> Reservation([FromBody] ParkingReservation request)
        {
            if (request == null || string.IsNullOrWhiteSpace(request.UserEmail))
            {
                return BadRequest("Neplatný požadavek.");
            }

            var reservation = await _database.CreateReservationAsync(request.ParkingLotId, request.StartTime, request.EndTime, request.LicensePlate, request.UserEmail);

            if (reservation == null)
            {
                return StatusCode(500, "Nepodařilo se vytvořit rezervaci");
            }
            await _database.SaveOldReservations();
            return Ok(reservation);

        }

        [HttpPut("Update")]
        public async Task<IActionResult> UpdateReservation([FromBody] ParkingReservation reservation)
        {
            if (reservation == null)
            {
                return BadRequest("Špatně zadaná rezervace");
            }

            await _database.UpdateReservationAsync(reservation);
            return Ok(reservation);
        }

        [HttpGet("Stats")]
        public async Task<IActionResult> ShowStats()
        {
            List<EndedReservationsCount> count = (List<EndedReservationsCount>)await _database.GetParkingStatisticsAsync();
            
            return Ok(count);
        }

        [HttpGet("History/{spotid}")]
        public async Task<IActionResult> ShowSpotHistory(int spotid)
        {
            List<ParkingStatusHistory> list = (List<ParkingStatusHistory>)await _database.GetHistoryOfParkingSpotAsync(spotid);
        
            return Ok(list);
        }

        [HttpDelete("Delete/{id}")]
        public async Task<IActionResult> DeleteReservation(int id)
        {
            await _database.DeleteReservationAsync(id);
            return Ok();
        }

        [HttpGet("GetReservations")]

        public async Task<IActionResult> GetAllReservations()
        {
            var list = await _database.GetAllReservationsAsync();

            return Ok(list);
        }

        [HttpPut("UpdateDetail")]
        public async Task<IActionResult> UpdateDetailSpot([FromBody] ParkingSpot model)
        {
            await _database.EditStatusAsync(model.Id, model.ManualStatus);
            return Ok();
        }


        [HttpPost("AddSpot/{lotid}")]
        
        public async Task<IActionResult> AddParkingSpotAsync(int lotid)
        {
            await _database.AddParkingSpot(lotid);

            return Ok();
        }

        [HttpDelete("DeleteSpot/{spotid}")]

        public async Task<IActionResult> DeleteParkingSpotAsync(int spotid)
        {
            await _database.DeleteParkingSpot(spotid);

            return Ok();
        }

        


        [HttpGet("ApiDescription")]

        public IActionResult Description()
        {

            var controller = typeof(ApiParkingLotController);
            var actions = controller.GetMethods(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.DeclaredOnly);
            
            List<object> endpoints = new List<object>();

            foreach (var action in actions)
            {
                var httpattribute = action.GetCustomAttributes().FirstOrDefault(attr => attr is HttpPostAttribute || attr is HttpGetAttribute || attr is HttpPutAttribute || attr is HttpDeleteAttribute);

                string httpAction = httpattribute switch
                {
                    HttpDeleteAttribute => "Delete",
                    HttpGetAttribute => "Get",
                    HttpPostAttribute => "Post",
                    HttpPutAttribute => "Put",
                    _ => ""
                };

                string route = (httpattribute as HttpMethodAttribute)?.Template ?? "";

                var parametres = action.GetParameters().Select(p => new
                {
                    Name = p.Name,
                    Type = p.ParameterType.Name,
                }).ToList();


                endpoints.Add(new
                {
                    Route = $"ApiParkingLot/{route}".TrimEnd('/'),
                    HttpMethod = httpAction,
                    MethodName = action.Name,
                    Parametres = parametres,
                    ReturnType = action.ReturnType.Name,

                });
            }

            return Ok(endpoints);
        
        }
    }
}
