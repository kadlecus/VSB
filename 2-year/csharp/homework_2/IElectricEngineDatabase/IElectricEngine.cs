namespace IElectricEngineDatabase
{
    public interface IElectricEngine
    {
        public double BatteryCapacity { get; }
        public double RemainingEnergy {  get; }
        public void Charge(double quantity);
    }
}
