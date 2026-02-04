using desktop_projekt.Model;
using desktop_projekt.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace desktop_projekt
{
    /// <summary>
    /// Interakční logika pro UpdateReservation.xaml
    /// </summary>
    public partial class UpdateReservation : Window
    {
        private ReservationRequest ReservationRequest;
        private readonly ParkingLotService _parkingLotService;
        public UpdateReservation(ReservationRequest request)
        {
            InitializeComponent();
            ReservationRequest = request;
            _parkingLotService = new ParkingLotService();
        }

        private async void SendUpdatedReservation_Click(object sender, RoutedEventArgs e)
        {
          

            if (StartDatePicker.SelectedDate == null || EndDatePicker.SelectedDate == null)
            {
                MessageBox.Show("Zadejte prosím obě data.");
                return;
            }

            if (!TimeSpan.TryParse(StartTimeTextBox.Text, out var startTime) || !TimeSpan.TryParse(EndTimeTextBox.Text, out var endTime))
            {
                MessageBox.Show("Zadejte čas ve formátu HH:mm (např. 14:30).");
                return;
            }

            DateTime start = StartDatePicker.SelectedDate.Value.Date + startTime;
            DateTime end = EndDatePicker.SelectedDate.Value.Date + endTime;

            if (start > end)
            {
                MessageBox.Show("Začínající čas nemůže být dříve než končící");
                return;
            }

            if (end < DateTime.Now)
            {
                MessageBox.Show("Rezervace nemůže končit v minulosti");
                return;
            }

            ReservationRequest newReservation = new ReservationRequest
            {
                Id = ReservationRequest.Id,
                ParkingLotId = ReservationRequest.ParkingLotId,
                ParkingSpotId = ReservationRequest.ParkingSpotId,
                LicensePlate = LicensePlate.Text,
                StartTime = start,
                EndTime = end,
                UserEmail = Email.Text,
            };


            if (await _parkingLotService.UpdateReservationAsync(newReservation))
            {
                MessageBox.Show($"Povedlo se aktualizovat rezervaci {newReservation.Id}");


            }
            else
            {
                MessageBox.Show("Nepovedlo se aktualizovar rezervaci");
            }
        
        
        }

    }
}
