namespace Project.Models
{
    public class ParkingStatusHistory
    {
        public int Id { get; set; }

        public int ParkingSpotId { get; set; }

        public int ParkingLotId { get; set; }

        public string NewStatus { get; set; }

        public DateTime ChangedAt { get; set; }

    }
}
