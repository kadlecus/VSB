using System.ComponentModel.DataAnnotations.Schema;

namespace Project.Models
{
    public class ParkingReservation
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
        public decimal CalculatePriceForDuration() 
        {
            decimal halfhours = Math.Ceiling((decimal)Duration / (decimal)30);

            const decimal pricePerHour = 20;

            return (decimal) (halfhours * pricePerHour);
        }

        [NotMapped]  
        public string? ParkingLotName { get; set; }
    }
}
