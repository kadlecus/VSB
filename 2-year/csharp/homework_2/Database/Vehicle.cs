namespace Database
{
    public abstract class Vehicle
    {
        public string? VehicleName { get; set; }
        public double AccountBalance { get; set; }

        public virtual void DisplayInfo() {

            if (VehicleName != null)
            {
                Console.WriteLine(VehicleName + " | " + AccountBalance + " Kč\n");
            }else
            {
                Console.WriteLine("Unknown" + " | " + AccountBalance + " Kč\n");
            }
        }

        public abstract int NumberOfWheels();
    }
}
