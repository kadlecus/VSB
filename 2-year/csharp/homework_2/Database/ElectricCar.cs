using IElectricEngineDatabase;
namespace Database
{
    public class ElectricCar : Car, IElectricEngine
    {
        public double BatteryCapacity { get; init; }
        public double RemainingEnergy { get; private set; }

        private int numberOfWheel;
        public int NumberOfWheel
        {

            get { return numberOfWheel; }

            set
            {

                numberOfWheel = NumberOfWheels();
            }

        }
        public void Charge(double quantity)
        {

            this.RemainingEnergy += quantity;
            if (this.RemainingEnergy > this.BatteryCapacity)
            {
                this.RemainingEnergy = this.BatteryCapacity;
            }
        }
            public override void DisplayInfo()
            {
            base.DisplayInfo();
            
            Console.WriteLine("Stav baterie: " + Math.Round((this.RemainingEnergy / this.BatteryCapacity) * 100) + "%\n");

            }
        
    }
}

