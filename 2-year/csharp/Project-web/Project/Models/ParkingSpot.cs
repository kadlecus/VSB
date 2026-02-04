using System.ComponentModel.DataAnnotations.Schema;

namespace Project.Models
{
    public class ParkingSpot
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

    
        public string ManualStatus { get; set; }
        public string Status { get; set; }
     

    }
}
