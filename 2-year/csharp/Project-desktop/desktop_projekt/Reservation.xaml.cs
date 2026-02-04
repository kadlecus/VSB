using System;
using System.Collections.Generic;
using System.Linq;
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
using desktop_projekt.Services;
using desktop_projekt.Model;
using System.Net.Http;
using System.Net.Http.Json;

namespace desktop_projekt
{
    /// <summary>
    /// Interakční logika pro Reservation.xaml
    /// </summary>
    public partial class Reservation : Window
    {
        private readonly ParkingLotService _parkingLotService;
        private readonly int LotId;
        public Reservation(int id)
        {
            InitializeComponent();
            _parkingLotService = new ParkingLotService();
            LotId = id; 
        }


        private async void SendReservation_Click(object sender, RoutedEventArgs e)
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

            var reservation = new
            {
                ParkingLotId = LotId,
                LicensePlate = LicensePlate.Text,
                StartTime = start,
                EndTime = end,
                UserEmail = Email.Text,
              
            };

            using HttpClient client = new HttpClient();

            try { 
                var response = await client.PostAsJsonAsync("https://localhost:7020/ApiParkingLot/Reservation/", reservation);
                string responseText = await response.Content.ReadAsStringAsync();
                MessageBox.Show(responseText);
                if (response.IsSuccessStatusCode)
                {
                    MessageBox.Show("Rezervace se úspěšně vytvořila");
                   
                    this.Close();
                }   
                else 
                {
                    MessageBox.Show("Rezervace se nepovedla");

                }
            }
            catch (HttpRequestException httpEx)
            {
                MessageBox.Show($"Chyba HTTP: {httpEx.Message}");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Výjimka: {ex.Message}");
            }
        }
    }
}
