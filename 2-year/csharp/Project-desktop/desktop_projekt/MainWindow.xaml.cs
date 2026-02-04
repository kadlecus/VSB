using desktop_projekt.Model;
using desktop_projekt.Services;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace desktop_projekt
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ParkingLotService _parkingLotService;
        public MainWindow()
        {
            InitializeComponent();
            _parkingLotService = new ParkingLotService();
        }

        private async void LoadParkingLotsButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
          
                List<ParkingLot> parkingLots = await _parkingLotService.GetParkingLotsAsync();

             
                ParkingLotDataGrid.ItemsSource = parkingLots;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Došlo k chybě: {ex.Message}");
            }
        }

        private async void DetailButton_Click(object sender, RoutedEventArgs e)
        {
            var button = (Button) sender;
            if (button.Tag is int lotId)
            {
                var spotsdetail = await _parkingLotService.GetParkingSpotsAsync(lotId);
                ParkingSpotDataGrid.ItemsSource = spotsdetail;
            }

        }

        private void ReservationButton_Click(object sender, RoutedEventArgs e)
        {
            var button = (Button)sender;
            if(button?.Tag is int Id)
            {
              var reservation = new Reservation(Id);
                reservation.Show();
            }
        
        }

        private void ShowAllReservations_Click(object sender, RoutedEventArgs e)
        {
            var edit = new EditReservations();
            edit.Show();
        
        }

        private async void AddSpot_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;

            if (button.Tag is int Id)
            {
                if (await _parkingLotService.AddParkingSpot(Id))
                {
                    MessageBox.Show("Povedlo se pridat misto");

                }
                else
                {
                    MessageBox.Show("Neco se nepovedlo");
                }
            }
        
        }

        private async void DeleteSpot_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;
            if (button.Tag is int Id)
            {
                if (await _parkingLotService.DeleteParkingSpotAsync(Id))
                {
                    MessageBox.Show("Povedlo se odstranit místo");
                }else
                {
                    MessageBox.Show("Něco se nepovedlo");
                }
            
            }
        
        }

        private async void EditManualStatus_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;
            if (button.DataContext is ParkingSpot spot)
            {
                try
                {
                    if (spot.Status != "Obsazené")
                    {
                        if (spot.ManualStatus == "Dostupné")
                        {
                            spot.ManualStatus = "V údržbě";
                        }
                        else
                        {
                            spot.ManualStatus = "Dostupné";
                        }
                        await _parkingLotService.UpdateManualStatusAsync(spot);
                        MessageBox.Show("Úspěšně se provedla změna.");
                    }
                    else
                    {
                        MessageBox.Show("Změna se nepovedla. Místo je nejspíše obsazené");
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
          
            }
        
        }

        private async void SpotHistory_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;
            
            if(button.Tag is int id)
            {
                List<ParkingSpotHistory?> list =  await _parkingLotService.GetParkingSpotHistory(id);
                ParkingHistorySpotDataGrid.ItemsSource = list;

            }
        
        }

    }
}