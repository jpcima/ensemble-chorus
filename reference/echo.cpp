/* echo.cpp - Model of a Bucket Bridage Device delay.
    Requires STK, https://ccrma.stanford.edu/software/stk/
    Written in 2010 by Colin Raffel
    For more information, please visit https://ccrma.stanford.edu/~craffel/software/bbdmodeling/
*/

#include "RtAudio.h"
#include "Delay.h"
#include "Iir.h"
#include <stdlib.h>
#include <math.h>
#include "Noise.h"


using namespace stk;
using namespace std;

StkFloat mix;
StkFloat regen;
StkFloat delayTime;
StkFloat nStages;
Delay *bbd;
StkFloat clockRate;
StkFloat T;
StkFloat currTime;
StkFloat clockTime;
Iir *antiAliasFilter;
Iir *reconstructionFilter1;
Iir *reconstructionFilter2;
Iir *compressorAverager;
Iir *expanderAverager;
Noise *noise;

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  
  // Value from input
  static StkFloat inVal = 0;
  // Previous input
  static StkFloat outVal = 0;
  // Previous compressor value
  static StkFloat prevCompOut = 1.0;
  // Value to be input to BBD
  static StkFloat bbdIn = 0;
  // Value from BBD
  static StkFloat bbdOut = 0;
  // Reconstructed out
  static StkFloat reconstructedOut = 0;
  // Previous BBD value for reconstruction
  static StkFloat prevBbdOut = 0;
  // Previous input value
  static StkFloat prevInVal = 0;
  
  register StkFloat *oSamples = (StkFloat *) outputBuffer, *iSamples = (StkFloat *) inputBuffer;
  
  for ( unsigned int i=0; i<nBufferFrames; i++ ) {
    
    inVal = *iSamples++;
    // Compress
    bbdIn = ( .5*inVal + prevBbdOut ) / ( compressorAverager->tick( fabs(prevCompOut) ) + .00001);
    // Remember compressor output
    prevCompOut = bbdIn;
    // Anti-aliasing filter
    bbdIn = antiAliasFilter->tick( bbdIn );
    // Sampled input/output
    if( currTime > clockTime )
    {
      // cerr << "yes" << endl;
      // Tick in linearly interpolated value, get out value
      bbdOut = bbd->tick( (( currTime - clockTime )/T)*bbdIn + (1 - ( currTime - clockTime )/T)*prevInVal );
      // cerr << (( currTime - clockTime )/T) << endl;
      // Decrement time
      currTime -= clockTime;
    }
    /*else
      cerr << "no" << endl;*/
    
    //bbdOut = bbd->tick( bbdIn );
    /*BBD nonlinearity
    if ( bbdOut > 1 )
    {
      bbdOut = 1 - 1/8 - 1/18;
      cerr << "BBD clip high" << endl;
    }
    else if (bbdOut < -1 )
    {
      bbdOut = -1 - 1/8 + 1/18;
      cerr << "BBD clip low" << endl;
    }    
    else*/
      bbdOut = bbdOut - (1.0/166.0)*bbdOut*bbdOut - (1.0/32.0)*bbdOut*bbdOut*bbdOut + 1.0/16.0;
    // Add in -60 dB noise
    bbdOut = bbdOut + 0.001*noise->tick();
    
    // Reconstruction filters
    reconstructedOut = reconstructionFilter1->tick( bbdOut );
    reconstructedOut = reconstructionFilter2->tick( reconstructedOut );
    // Expand
    reconstructedOut = reconstructedOut * expanderAverager->tick( fabs(reconstructedOut) );
    
    /* Soft clipping
    if ( reconstructedOut > 1 )
      outVal = .6666666666;
    else if ( reconstructedOut < -1 )
      outVal = -.6666666666;
    else
      outVal = reconstructedOut - reconstructedOut*reconstructedOut*reconstructedOut/3;*/
    
    outVal = reconstructedOut;
    // Mix
    outVal = ( (1 - mix)*inVal + 2*mix*outVal );
    // Write out
    *oSamples++ = outVal;
    *oSamples++ = outVal;
    // Remember output
    prevBbdOut = regen*reconstructedOut;
    // Remember input
    prevInVal = bbdIn;
    // Increment time
    currTime += T;
    
  }
  return 0;
}

int main(int argc,char *argv[])
{
  
  ////////////////////////
  // Delay lines and mixing
  ////////////////////////
  
  // Set default mix
  mix = .5;
  // Set default regeneration amount
  regen = .02;
  // Set default echo time, 300 ms (in seconds)
  delayTime = .4;
  // Get 2nd, 3rd and 4th command line arguments for mix, regen, and delay time
  if ( argc > 1 )
    mix = atof(argv[1]);
  if ( argc > 2 )
    regen = atof(argv[2]);
  if ( argc > 3 )
    delayTime = atof(argv[3]);
  
  // Set default BBD length
  nStages = 4096;
  if ( argc > 6 )
    nStages = atof(argv[6]);
  
  // Create delay line
  bbd = new Delay( nStages, nStages );
  // bbd = new Delay( delayTime*Stk::sampleRate(), delayTime*Stk::sampleRate() );
  
  // Calculate clock rate (Hz)
  clockRate = nStages/(2.0*delayTime);
  cerr << clockRate << endl;
  
  // Noise
  noise = new Noise( 0 );
  
  
  ////////////////////////
  // Timing variables
  ////////////////////////
  
  // Sampling interval
  T = 1/(Stk::sampleRate());
  // Current time
  currTime = 0;
  // Clocking interval
  clockTime = 1/clockRate;
  
  
  ////////////////////////
  // Filters
  ////////////////////////
  
  // Coefficients for anti-aliasing filter (calculated in MatLab)
  StkFloat antiAliasBCoefVals[8] = { 0.00002808162654240159, 0.00324260184959550063, 0.01229358405255225224, 0.01781338547909389058, 0.01191661427670600605, 0.00351674566780364349, 0.00034109599239625331, 0.00000665645917792087 };
  StkFloat antiAliasACoefVals[8] = { 1.00000000000000000000, 0.27002485476511250972, -2.35210121584816533868, -0.47624252782494447267, 1.85825180320240179732, 0.30789821629786440216, -0.48145639585378052772, -0.07721597509005941051 };

  std::vector< StkFloat > antiAliasBCoefs( antiAliasBCoefVals, antiAliasBCoefVals+8 );
  std::vector< StkFloat > antiAliasACoefs( antiAliasACoefVals, antiAliasACoefVals+8 );
  // Anti-aliasing filter
  antiAliasFilter = new Iir( antiAliasBCoefs, antiAliasACoefs );
  
  // Coefficients for reconstruction filter 1 (calculated in MatLab)
  StkFloat reconstruction1BCoefVals[6] = { 0.00051516013318437094, 0.03041821522444834724, 0.06752981661722373685, 0.04770540623300938837, 0.01033396947561467973, 0.00001106873510238544 };
  StkFloat reconstruction1ACoefVals[6] = { 1.00000000000000000000, -0.63300309066683380088, -1.16456643091420186664, 0.76529508607787266605, 0.33776127751256762588, -0.14897208486870763822 };
  std::vector< StkFloat > reconstruction1BCoefs( reconstruction1BCoefVals, reconstruction1BCoefVals+6 );
  std::vector< StkFloat > reconstruction1ACoefs( reconstruction1ACoefVals, reconstruction1ACoefVals+6 );
  // Reconstruction filter 1
  reconstructionFilter1 = new Iir( reconstruction1BCoefs, reconstruction1ACoefs );
  
  // Coefficients for reconstruction filter 2 (calculated in MatLab)
  StkFloat reconstruction2BCoefVals[3] = { 0.01440296150822061375, 0.12374170373521460597, 0.01322614050481232296 };
  StkFloat reconstruction2ACoefVals[3] = { 1.00000000000000000000, -1.75672492751137410139, 0.90805921207607520618 };
  std::vector< StkFloat > reconstruction2BCoefs( reconstruction2BCoefVals, reconstruction2BCoefVals+3 );
  std::vector< StkFloat > reconstruction2ACoefs( reconstruction2ACoefVals, reconstruction2ACoefVals+3 );
  // Reconstruction filter 2
  reconstructionFilter2 = new Iir( reconstruction2BCoefs, reconstruction2ACoefs );
  
  
  ////////////////////////
  // Compander
  ////////////////////////
  
  // Set default compander cap value in uF
  double companderCrect = .82;
  // Read in compander cap value if it exists
  if ( argc > 6 )
    companderCrect = atof(argv[6]);
  
  // Get compander rectifier smoothing factor
  // http://en.wikipedia.org/wiki/Low-pass_filter
  StkFloat companderSmoothing = ( T )/( 10000*companderCrect*.000001 + T );
  // Coefficients for averaging filter
  StkFloat averagerBCoefVals[2] = { companderSmoothing, 0.0 };
  StkFloat averagerACoefVals[2] = { 1.0, -1+companderSmoothing };
  std::vector< StkFloat > averagerBCoefs( averagerBCoefVals, averagerBCoefVals+2 );
  std::vector< StkFloat > averagerACoefs( averagerACoefVals, averagerACoefVals+2 );
  // Create compressor averager
  compressorAverager = new Iir( averagerBCoefs, averagerACoefs );
  // Create expander averager
  expanderAverager = new Iir( averagerBCoefs, averagerACoefs );
  
  
  // Allocate the adac here.
  RtAudio adac;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  RtAudio::StreamParameters oparameters, iparameters;
  oparameters.deviceId = adac.getDefaultOutputDevice();
  oparameters.nChannels = 2;
  iparameters.deviceId = adac.getDefaultInputDevice();
  iparameters.nChannels = 1;
  unsigned int bufferFrames = RT_BUFFER_SIZE;
  try {
    adac.openStream( &oparameters, &iparameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, NULL );
  }
  catch ( RtError& error ) {
    error.printMessage();
    goto cleanup;
  }
  
  // If realtime output, set our callback function and start the dac.
  try {
    adac.startStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
    goto cleanup;
  }
  
  // Setup finished.
  while ( true ) {
    // Periodically check "done" status.
    Stk::sleep( 50 );
  }
  
cleanup:
  delete bbd;
  delete antiAliasFilter;
  delete reconstructionFilter1;
  delete reconstructionFilter2;
  delete compressorAverager;
  delete expanderAverager;
  return(0);
}
