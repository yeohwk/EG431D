#include <FIR.h>
#define SOUND_SENSOR A2           // Grove - Sound Sensor connects to Arduino Analog A2 Port

// FIR filter design with: http://t-filter.engineerjs.com/
// FIR Filter Coefficients - Begin
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 3000 Hz

fixed point precision: 16 bits

* 0 Hz - 200 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

* 300 Hz - 500 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 600 Hz - 1500 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define FILTER_TAP_NUM 39

static int filter_taps[FILTER_TAP_NUM] = {
  298,
  236,
  27,
  -257,
  -342,
  -162,
  -11,
  -219,
  -634,
  -567,
  510,
  2098,
  2710,
  1121,
  -2166,
  -4816,
  -4395,
  -616,
  4039,
  6123,
  4039,
  -616,
  -4395,
  -4816,
  -2166,
  1121,
  2710,
  2098,
  510,
  -567,
  -634,
  -219,
  -11,
  -162,
  -342,
  -257,
  27,
  236,
  298
};
// FIR Filter Coefficients - End

FIR<int, FILTER_TAP_NUM> fir_f;
void setup()
{ 
    Serial.begin(115200);
    // Initialize Analog Resolution to 10 bits (for speed)
    analogReadResolution(10);
    fir_f.setFilterCoeffs(filter_taps);
    Serial.print("FIR Low Pass Filter Gain: ");
    Serial.println(fir_f.getGain());
}

void loop()
{
    long sound = 0;
    long filtered_sound = 0;
    sound = analogRead(SOUND_SENSOR);
    filtered_sound = fir_f.processReading(sound);
    Serial.print(sound);     
    Serial.print(" ");     
    Serial.println(filtered_sound);          
}
