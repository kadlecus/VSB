using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace desktop_projekt.Model
{
    public class ParkingSpotHistory
    {
        public int Id { get; set; }

        public int ParkingSpotId { get; set; }

        public int ParkingLotId { get; set; }

        public string NewStatus { get; set; }

        public DateTime ChangedAt { get; set; }

    }
}
