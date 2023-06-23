#include <QtCharts/QtCharts>
#include <QtWidgets/QtWidgets>
#include <fftw3.h>
#include <math.h>
#include <portaudio.h>
#include <stdio.h>

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 60
#define BIN_COUNT 31

fftw_complex *fftwInput;
fftw_complex *fftwOutput;
fftw_plan fftPlan;

PaStream *stream;

class QRTAView : public QChartView {
public:
  virtual void closeEvent(QCloseEvent *event) {
    // Stop and close the stream
    PaError err = Pa_StopStream(stream);
    if (err != paNoError) {
      printf("Failed to stop PortAudio stream: %s\n", Pa_GetErrorText(err));
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
      printf("Failed to close PortAudio stream: %s\n", Pa_GetErrorText(err));
    }

    // Cleanup and shutdown
    fftw_destroy_plan(fftPlan);
    fftw_free(fftwInput);
    fftw_free(fftwOutput);
    Pa_Terminate();

    QChartView::closeEvent(event);
  }
};

int processAudio(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags, void *userData) {
  
  const float *input = (const float *)inputBuffer;
  QBarSet *set = (QBarSet*)userData;

  // Copy audio samples to FFT input buffer
  for (int i = 0; i < BUFFER_SIZE; i++) {
    fftwInput[i][0] = input[i];
    fftwInput[i][1] = 0.0;
  }

  // Perform forward FFT
  fftw_execute(fftPlan);

  // Analyze frequency bins
  // Example: Calculate the magnitude spectrum and print the value for a
  // specific frequency bin
  for (int bin = 0; bin < BUFFER_SIZE; bin++) {
    double magnitude = sqrt(fftwOutput[bin][0] * fftwOutput[bin][0] +
                            fftwOutput[bin][1] * fftwOutput[bin][1]);

    //printf(" %d", dB);
    set->replace(bin, magnitude);
  }
  //printf("\n");

  return paContinue;
}

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  // Initialize PortAudio
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    printf("Failed to initialize PortAudio: %s\n", Pa_GetErrorText(err));
    return 1;
  }

  // Create FFTW plans and buffers
  fftwInput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * BUFFER_SIZE);
  fftwOutput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * BUFFER_SIZE);
  fftPlan = fftw_plan_dft_1d(BUFFER_SIZE, fftwInput, fftwOutput, FFTW_FORWARD,
                             FFTW_MEASURE);

  // Create a Qt Charts chart view
  QRTAView chartView;

  // Create a Qt Charts bar chart
  QChart chart;
  QBarSeries series;

  // Add data to the bar series
  QBarSet set = QBarSet("");
  for (int bin = 0; bin < BIN_COUNT; bin++)
    set << bin;
  series.append(&set);
  chart.addSeries(&series);

  // Create the default axes for the bar chart
  QCategoryAxis axisX;
  QValueAxis axisY;
  axisY.setMax(10.0);
  chart.addAxis(&axisX, Qt::AlignBottom);
  chart.addAxis(&axisY, Qt::AlignLeft);
  series.attachAxis(&axisX);
  series.attachAxis(&axisY);

  // Set the chart view properties
  chart.setTheme(QChart::ChartThemeDark);
  chart.setAnimationOptions(QChart::AllAnimations);
  chart.setAnimationDuration(500);
  chartView.setChart(&chart);
  chartView.setWindowTitle("Audio RTA");
  chartView.resize(1920, 1080);
  chartView.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  chartView.show();

  // Open PortAudio stream
  err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, BUFFER_SIZE,
                             processAudio, (void*)&set);
  if (err != paNoError) {
    printf("Failed to open PortAudio stream: %s\n", Pa_GetErrorText(err));
    return 1;
  }

  // Start the stream
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    printf("Failed to start PortAudio stream: %s\n", Pa_GetErrorText(err));
    return 1;
  }

  // Wait for user input to stop the program (you can modify this as needed)
  printf("Running real-time audio analyzer. Press Enter to stop.\n");
  getchar();

  return app.exec();
}
