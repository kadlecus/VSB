using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace desktop_projekt.Model
{
    public class ReservationRequest
    {
     
            public int Id { get; set; } 
            public int ParkingLotId { get; set; }

            public int ParkingSpotId { get; set; }
            public string LicensePlate { get; set; }
            public DateTime StartTime { get; set; }
            public DateTime EndTime { get; set; }
            public decimal Price { get; set; }
        
            public int Duration { get; set; }
            public string UserEmail { get; set; }
        

    }

}
