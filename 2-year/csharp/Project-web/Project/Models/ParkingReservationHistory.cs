using System.ComponentModel.DataAnnotations.Schema;

namespace Project.Models
{
    public class ParkingReservationHistory
    {
        private int id;
        public int Id
        {
            get
            {
                return id;
            }

            set
            {
                if (0 > value)
                {
                    throw new ArgumentOutOfRangeException(nameof(Id), "Index out of range.");
                }
                id = value;

            }
        }
        public int ParkingLotId { get; set; }
        public int ParkingSpotId { get; set; }
        public string LicensePlate { get; set; }
        public DateTime StartTime { get; set; }
        public DateTime EndTime { get; set; }
        public int Duration { get; set; }
        public decimal Price { get; set; }
        public string UserEmail { get; set; }

        [NotMapped]
        public string ParkingLotName { get; set; }
    } 

}
