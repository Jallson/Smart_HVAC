 /***
 * Some of this code came from edge impulse's example and                           *
 * modifications to M. Marcial's code.                                              *
 * Hence, the following Edge Impulse and M. Marcial's copyright statement.          *
 *----------------------------------------------------------------------------------*
 * Edge Impulse Arduino examples                                                    *
 * Copyright (c) 2021 EdgeImpulse Inc.                                              *
 * M. Marcial "Spresense-VisionModel.ino"                                           *
 * Copyright (c) 2022 M. Marcial                                                    *
 *                                                                                  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy     *
 * of this software and associated documentation files (the "Software"), to deal    *
 * in the Software without restriction, including without limitation the rights     *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
 * copies of the Software, and to permit persons to whom the Software is            *
 * furnished to do so, subject to the following conditions:                         *
 *                                                                                  *
 * The above copyright notice and this permission notice shall be included in       *
 * all copies or substantial portions of the Software.                              *
 *                                                                                  *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    * 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    *
 * SOFTWARE.                                                                        *
 *                                                                                  *
 ************************************************************************************
 ***/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      MEMORY INFO                                                                               //
//                      The program requires a minimum MainCore memory of 1024 kB                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***
//------------------------------------------------------------------------------------------------------------------
  Edge Impulse Studio float32 Library Estimates: RAM 131k, Flash 182
//------------------------------------------------------------------------------------------------------------------

  Target 1152 kB
  *** ---> Works!
  Sketch uses           393,424 bytes (33%) of program storage space.
  Maximum is          1,179,648 bytes.
  Global variables use  393,424 bytes (33%) of dynamic memory,
  leaving               786,224 bytes for local variables.
  Maximum is          1,179,648 bytes.

  Target 1024 kB
  *** ---> Works!
  Sketch uses           393,392 bytes (37%) of program storage space.
  Maximum is          1,048,576 bytes.
  Global variables use  393,392 bytes (37%) of dynamic memory,
  leaving               655,184 bytes for local variables.
  Maximum is          1,048,576 bytes.
  
  Target 896 kB
  *** --->  ERR: failed to allocate tensor arena
                 Failed to allocate TFLite arena (error code 1)
                 ERROR: Failed to run classifier in camera_classify(). Err=-6
  Sketch uses           393,336 bytes (42%) of program storage space.
  Maximum is            917,504 bytes.
  Global variables use  393,336 bytes (42%) of dynamic memory,
  leaving               524,168 bytes for local variables.
  Maximum is            917,504 bytes.                 

  Target 768 kB
  *** ---> Executes CamCB() once then crashes
  Sketch uses           393,336 bytes (50%) of program storage space.
  Maximum is            786,432 bytes.
  Global variables use  393,336 bytes (50%) of dynamic memory,
  leaving               393,096 bytes for local variables.
  Maximum is            786,432 bytes.
  
  Target 640 kB
  *** ---> Executes CamCB() once then crashes
  Sketch uses            393,336 bytes (60%) of program storage space.
  Maximum is             655,360 bytes.
  Global variables use   393,336 bytes (60%) of dynamic memory,
  leaving                262,024 bytes for local variables.
  Maximum is             655,360 bytes.
  
  Target 512 kB
  *** ---> Gets to loop(), then crashes.
  Sketch uses           393,336 bytes (74%) of program storage space.
  Maximum is            524,288 bytes.
  Global variables use  393,336 bytes (74%) of dynamic memory,
  leaving               130,952 bytes for local variables.
  Maximum is            524,288 bytes.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      PROGRAMMING INFO                                                                          //
//                      If Arduino fails to program the Spresense, execute the following command line.            //
//                      Change "<your_path>" depending on your system configuration                               //
//                      Change "/dev/ttyUSB0" to serial port the Spresense connected to.                          //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  My Ardy Build Folder: C:\Users\mmarc\AppData\Local\Temp\arduino-sketch-FCC2488F5F48E4ED1B9D9484E3902BF5
  
  Flash from Ubuntu:
      "<your_path>/.arduino15/packages/SPRESENSE/tools/spresense-tools/2.6.0/flash_writer/linux/flash_writer"
      -s -c
      "/dev/ttyUSB0"
      -d -n
      "/tmp/arduino-sketch-<a_randon_32-bit_hex_value>>/Spresese-CameraModel.ino.spk"
  
  Flash from Windows:
    "C:\Users\<username>\AppData\Local\Arduino15\packages\SPRESENSE\tools\spresense-tools\2.6.0/flash_writer/windows/flash_writer.exe"
    -s -c
    "COM11"
    -d -n
    "C:\Users\<username>\AppData\Local\Temp\arduino-sketch-<a_randon_32-bit_hex_value>/Spresense-AudioModel.ino.spk"
//------------------------------------------------------------------------------------------------------------------
***/

/* Defines & Includes ------------------------------------------------------------------------------------------- */
// The following are not supported on a SubCore (there may be others):
//                                                                      Audio.h,
//                                                                      Camera.h,
//                                                                      File.h,
//                                                                      MemoryUtil.h
#ifdef SUBCORE
#error "Core selection is wrong!! Must compile to MainCore!!!"
#endif

///#define EI_CLASSIFIER_HAS_MODEL_VARIABLES       1
///#define EI_CLASSIFIER_INPUT_FRAMES              1
///#define EI_CLASSIFIER_INTERVAL_MS               1
///#define EI_CLASSIFIER_TFLITE_OUTPUT_DATA_TENSOR 1 //0
///#define EI_CLASSIFIER_FULL_TFLITE               1 //NA
///#define EI_CLASSIFIER_OBJECT_DETECTION          0 //1
//#include <Cup_inferencing.h>                // Either int8 or float model exported from the Edge Impulse Studio as an Arduino library works.
#include <Smart_HVAC_inferencing.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Camera.h>                         // Sony's camera library.

/* The following #defines define the center, crop, and resize of the image to the Impulse image size.
   We will use the Spresense hardware accelerator inside the CPU CXD5602.
   NOTE: EI_CLASSIFIER_INPUT width and height must be less than RAW_HEIGHT * SCALE_FACTOR, and must
   simultaneously meet the requirements of the Spresense api:
   https://developer.sony.com/develop/spresense/developer-tools/api-reference/api-references-arduino/group__camera.html#ga3df31ea63c3abe387ddd1e1fd2724e97
*/
#define SCALE_FACTOR 1
// Camera Specs:
//              2608 (H) x 1960 (V) = approx. 5.11 M pixels
//              Y/C, RGB and JPEG

//The Spresense HW accelerator can only handle:
//  Maximum width  is  768 pixels.
//  Maximum height is 1024 pixels.

//#define RAW_WIDTH  CAM_IMGSIZE_QQVGA_H   // 160 <-- Classifies ok.
//#define RAW_HEIGHT CAM_IMGSIZE_QQVGA_V   // 120

#define RAW_WIDTH    CAM_IMGSIZE_QVGA_H    // 320 <-- Classifies ok.
#define RAW_HEIGHT   CAM_IMGSIZE_QVGA_V    // 240

//#define RAW_WIDTH  CAM_IMGSIZE_VGA_H     // 640 <--!!!Although the HW can handle this the app crashes!!!
//#define RAW_HEIGHT CAM_IMGSIZE_VGA_V     // 480

//#define RAW_WIDTH  CAM_IMGSIZE_HD_H      //1280 <-- Exceeds HW accellerator maximum width of 768 pixels. 
//#define RAW_HEIGHT CAM_IMGSIZE_HD_V      // 720   

//#define RAW_WIDTH  CAM_IMGSIZE_QUADVGA_H //1280 <-- Exceeds HW accellerator maximum width of 768 pixels. 
//#define RAW_HEIGHT CAM_IMGSIZE_QUADVGA_V // 960

//#define RAW_WIDTH  CAM_IMGSIZE_FULLHD_H  //1920 <-- Exceeds HW accellerator maximum width of  768 pixels. 
//#define RAW_HEIGHT CAM_IMGSIZE_FULLHD_V  //1080 <-- Exceeds HW accellerator maximum height of 1024 pixels. 

//#define RAW_WIDTH  CAM_IMGSIZE_3M_H      //2048 <-- Exceeds HW accellerator maximum width of  768 pixels. 
//#define RAW_HEIGHT CAM_IMGSIZE_3M_V      //1536 <-- Exceeds HW accellerator maximum height of 1024 pixels. 

// Verified program !!!crashes!!! with these settings:
//#define RAW_WIDTH  CAM_IMGSIZE_5M_H      //2560 <-- Exceeds HW accellerator maximum width of  768 pixels. 
//#define RAW_HEIGHT CAM_IMGSIZE_5M_V      //1920 <-- Exceeds HW accellerator maximum height of 1024 pixels. 

// We need to clip (crop) the raw image from the camera to be the same size that the Impulse was configured with.
// The following 4 #defines are used to calculate the image:	lefttop_x, lefttop_y, rightbottom_x, & rightbottom_y.
// What is the difference between clip and crop?
// Clip: To limit or reduce the extent of one dataset by the extents or boundary of another.
// Crop: In this context means to cut or trim an image or raster file.
#define CLIP_WIDTH  (EI_CLASSIFIER_INPUT_WIDTH  * SCALE_FACTOR) // EI_CLASSIFIER_INPUT_WIDTH  is defined by the EI Model in "model_metadata.h"
#define CLIP_HEIGHT (EI_CLASSIFIER_INPUT_HEIGHT * SCALE_FACTOR) // EI_CLASSIFIER_INPUT_HEIGHT is defined by the EI Model in "model_metadata.h"
#define OFFSET_X   ((RAW_WIDTH  - CLIP_WIDTH)  / 2)
#define OFFSET_Y   ((RAW_HEIGHT - CLIP_HEIGHT) / 2)

// Oled screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BARLENGTH 15
#define BARHEIGHT 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#if EI_CLASSIFIER_INPUT_WIDTH  > RAW_HEIGHT * SCALE_FACTOR
#error "EI_CLASSIFIER_INPUT_WIDTH not compatiable with Spresense hardware accelerator.
#endif

#if EI_CLASSIFIER_INPUT_HEIGTH > RAW_HEIGHT * SCALE_FACTOR
#error "EI_CLASSIFIER_INPUT_WIDTH not compatiable with Spresense hardware accelerator.
#endif

#define DEBUG_IT  false             // Enable for very verbose logging from Edge Impulse SDK.
                                    // Show features from raw data during the Classify().
#define GRAYSCALE true

#define CLASSIFIER_THRESHOLD  0.7   // We will take an action, like saving a snapshot to a SD card
                                    // if the Prediction is above this value.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      START GLOBALS                                                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Static Variables --------------------------------------------------------------------------------------------- */
       bool    StateLED0     = false;               //Global: Toggle when entering    CamCB()
       bool    StateLED1     = false;               //Global: Toggle before calling   camera_classify()
       bool    StateLED2     = false;               //Global: Toggles when we classify an image with high confidence.
       bool    StateLED3     = false;               //Global: Toggle when entering    ei_camera_cutout_get_data().
static CamImage            sized_img;               //Global: Instatiate a Sony class to control the image from the camera.
static ei_impulse_result_t ei_result = { 0 };       //Global: "results" of Classifier(). This doesn't need to be global.

/* Prototypes --------------------------------------------------------------------------------------------------- */
              void printCamErr(              enum CamErr);
              void CamCB(                    CamImage);
static inline void mono_to_rgb(              uint8_t, uint8_t *, uint8_t *, uint8_t *);
int                ei_camera_cutout_get_data(size_t, size_t, float *);
static        void camera_classify(          bool);
              void camera_start_continuous(  bool);
              void setup();
              void loop();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      END GLOBALS                                                                               //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      CAMERA HELPER ROUTINES                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************************************************
 * @brief      Print Camera Error Message
 * @details    
 * @param[in]  err, error number 
 * @return     void
*******************************************************************************************************************/

// initialize arrays
byte room[4] = {};
int line[4] = {16, 29, 42, 55};
int bar[6] = {18, 37, 56, 75, 94, 113};


void printCamErr(enum CamErr err)
{
  //ei_printf("[FLOW] Entering printCamErr(%d)...\n", err);
  
  ei_printf("Error: ");
  switch (err)
  {
    case CAM_ERR_NO_DEVICE:
      ei_printf("No Device.\n");
      break;
    case CAM_ERR_ILLEGAL_DEVERR:
      ei_printf("Illegal device error.\n");
      break;
    case CAM_ERR_ALREADY_INITIALIZED:
      ei_printf("Already initialized.\n");
      break;
    case CAM_ERR_NOT_INITIALIZED:
      ei_printf("Not initialized.\n");
      break;
    case CAM_ERR_NOT_STILL_INITIALIZED:
      ei_printf("Still picture not initialized.\n");
      break;
    case CAM_ERR_CANT_CREATE_THREAD:
      ei_printf("Failed to create thread.\n");
      break;
    case CAM_ERR_INVALID_PARAM:
      ei_printf("Invalid parameter.\n");
      break;
    case CAM_ERR_NO_MEMORY:
      ei_printf("No memory.\n");
      break;
    case CAM_ERR_USR_INUSED:
      ei_printf("Buffer already in use.\n");
      break;
    case CAM_ERR_NOT_PERMITTED:
      ei_printf("Operation not permitted.\n");
      break;
    default:
      ei_printf("Unknown camera error.\n");
      break;
  }
}

/*******************************************************************************************************************
 * @brief      Convert Monochrome Data to RGB Values
 * @param[in]  mono_data  The mono data
 * @param[out] r          red pixel value
 * @param[out] g          green pixel value
 * @param[out] b          blue pixel value
 *******************************************************************************************************************/
 static inline void mono_to_rgb(uint8_t mono_data, uint8_t *r, uint8_t *g, uint8_t *b)
{
  //ei_printf("[FLOW] Entering mono_to_rgb()...\n");
  uint8_t v = mono_data;
  *r = *g = *b = v;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      EDGE IMPULSE VISION ROUTINES                                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if GRAYSCALE == true
 
/*******************************************************************************************************************
 * @brief      Convert Grayscale to RGB
 * @details    This is the routine that gets the data for the Impulse "signal".
 * @param[in]  offset,  pixel offset of raw buffer
 * @param[in]  length,  number of pixels to convert
 * @param[out] out_buf, pointer to store output image
 * @return     0
 *******************************************************************************************************************/
int ei_camera_cutout_get_data(size_t offset, size_t length, float *out_ptr)
{
  //ei_printf("[FLOW] Entering Grayscale, ei_camera_cutout_get_data()...\n");    
  StateLED3 = !StateLED3;
  digitalWrite(LED3, StateLED3);
  
  size_t bytes_left = length;
  size_t out_ptr_ix = 0;

  uint8_t *buffer = sized_img.getImgBuff(); // Get grayscale image.

  // Read byte for byte.
  while (bytes_left != 0)
  {
    // Grab the pixel value.
    uint8_t pixel = buffer[offset];

    // Convert to R/G/B.
    uint8_t r, g, b;
    mono_to_rgb(pixel, &r, &g, &b);

    // Save RGB value as a float.
    float pixel_f = (r << 16) + (g << 8) + b;
    out_ptr[out_ptr_ix] = pixel_f;

    // ...and go to the next pixel.
    out_ptr_ix++;
    offset++;
    bytes_left--;
  }

  // RGB image buffer is now ready.
  return 0;
}

#else

/*******************************************************************************************************************
 * @brief      Convert RGB565 raw camera buffer to RGB888.
 * @details    This is the routine that gets the data for the Impulse "signal".
 *             This is from the "nano_ble33_sense_camera.ino" example.
 * @param[in]  offset,  pixel offset of raw buffer
 * @param[in]  length,  number of pixels to convert
 * @param[out] out_buf, pointer to store output image
*******************************************************************************************************************/
int ei_camera_cutout_get_data(size_t offset, size_t length, float *out_ptr)
{
    //ei_printf("[FLOW] Entering RGB565, ei_camera_cutout_get_data()...\n");    
    StateLED3 = !StateLED3;
    digitalWrite(LED3, StateLED3);

    size_t pixel_ix   = offset * 2; 
    size_t bytes_left = length;
    size_t out_ptr_ix = 0;

    // Grab the value and convert to RGB.
    uint8_t *buffer = sized_img.getImgBuff(); // Get RGB565 image.

    // Read byte for byte.
    while (bytes_left != 0)
    {
      uint16_t pixel = (buffer[pixel_ix]   << 8) | 
                        buffer[pixel_ix+1] <<0;
      uint8_t r, g, b;
      r = ( (pixel >> 11 ) & 0x1f) << 3;
      g = ( (pixel >> 5  ) & 0x3f) << 2;
      b =   (pixel & 0x1f)         << 3;

      // Then convert to "out_ptr" format.
      float pixel_f = (r << 16) + 
                      (g <<  8) + 
                      (b <<  0);
      out_ptr[out_ptr_ix] = pixel_f;

      // ...and go to the next pixel.
      out_ptr_ix++;
      pixel_ix+=2;
      bytes_left--;
    }

    // ...and done converting RGB565 raw camera buffer to RGB888.
    return 0;
}
#endif

/*******************************************************************************************************************
 * @brief      Runs Inference on the static "sized_image" Buffer using the provided Impulse.
 * @details    
 * @param[in]  debug, Show features created from raw data.
 * @return     void
*******************************************************************************************************************/
static void camera_classify(bool debug)
{
  //ei_printf("[FLOW] Entering camera_classify(%b)...\n", debug);
  
  // Setup "signal": sets the callback function on the "signal_t" structure to reference the inference buffer.
  ei::signal_t signal;
  signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  signal.get_data     = &ei_camera_cutout_get_data;                               // This tells the "signal" where to get the sampled data from.

  EI_IMPULSE_ERROR err = run_classifier(&signal, &ei_result, debug);
  
  if (err != EI_IMPULSE_OK)
  {
    ei_printf("ERROR: Failed to run classifier in camera_classify(). Err=%d\n", err);
    return;
  }
  
  // Print the Predictions
  if (true)
  {
    // Print the Predictions
    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms, Total: %d ms.): \n",
             ei_result.timing.dsp,  ei_result.timing.classification,  ei_result.timing.anomaly,
             ei_result.timing.dsp + ei_result.timing.classification + ei_result.timing.anomaly);
             
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    // FOMO Vision Model Details: Image Segmentation.
    bool bb_found = ei_result.bounding_boxes[0].value > 0;

    for (int x = 0; x < 4; x++) {
      room[x] = 0;
    }
    
    for (size_t ix = 0; ix < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; ix++)
    {
      auto bb = ei_result.bounding_boxes[ix];
      if (bb.value == 0) {
          continue;
      }

      ei_printf("    %s (", bb.label);
      ei_printf_float(bb.value);
      ei_printf(") [ x: %u, y: %u, width: %u, height: %u ]\n", bb.x, bb.y, bb.width, bb.height);
      if (bb.y < 44) {
        if (bb.x < 44) {
          room[0]++;
        }
        else {
          room[1]++;
        }
      }
      else {
        if (bb.x < 44) {
          room[2]++;
        }
        else {
          room[3]++;
        }
      }
    }

    // --- SEND TO I2C RECEIVER ---
    Wire.beginTransmission(9);
    for (int i = 0; i < 4; i++){
      Wire.write(room[i]);
    }
    Wire.endTransmission();
    // --- SEND TO RECEIVER CODE ENDS ---


    // print the no. of person in each rooms
    ei_printf("\n[ room 1: %d, room2: %d, room3: %d, room4: %d ]\n", room[0], room[1], room[2], room[3]);

    // --- PRINTING TO THE OLED STARTS HERE ---

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(10,0);
    display.println("TOTAL OCCUPANTS");
    display.setCursor(110,0);
    display.println(room[0]+room[1]+room[2]+room[3]);
    display.setCursor(0, line[0]);
    display.println("A");
    display.setCursor(0, line[1]);
    display.println("B");
    display.setCursor(0, line[2]);
    display.println("C");
    display.setCursor(0, line[3]);
    display.println("D");
    display.setCursor(37, line[3]);

    for (int i = 0; i < 4; i++){
      for (int j = 0; j < room[i]; j++){
        display.fillRect(bar[j], line[i], BARLENGTH, BARHEIGHT, WHITE);
      }
    }
    display.display();

    // --- OLED PRINT ENDS HERE ---

    if (!bb_found) {
      ei_printf("    No objects found\n");
    }
#else
    // Vision Model Details: Image Classification.

    // Determine max inference value so we can star it when we display the list of predictions.      
    uint8_t maxInferenceIdx = 0;
    for (size_t ix = 1; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
    {
      if( ei_result.classification[ix].value > ei_result.classification[maxInferenceIdx].value)
      {
        maxInferenceIdx = ix;
      }          
    }
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
    {
      if (maxInferenceIdx == ix)
      {
        // Highlight the label with the maximum prediction value.
        ei_printf("*** %s:\t\t", ei_result.classification[ix].label);
        ei_printf_float(ei_result.classification[ix].value);
        ei_printf(" ***\n");
      }
      else
      {      
        // Show other labels with their prediction value.
        ei_printf("    %s:\t\t", ei_result.classification[ix].label);
        ei_printf_float(ei_result.classification[ix].value);
        ei_printf("\n");
      }
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    // Handle Anomaly Details.
    ei_printf("    anomaly score: ");
    ei_printf_float(ei_result.anomaly);
    ei_printf("\n");
#endif
#endif
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      SPRESENSE VISION ROUTINES                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************************************************
 * @brief   Callback that checks for the presence of an image in the camera preview window.
 * @details 
 * @param
 * @return  void
 *******************************************************************************************************************/
void CamCB(CamImage img)
{
  //ei_printf("[FLOW] Entering CamCB()...\n");
  StateLED0 = !StateLED0;
  digitalWrite(LED0, StateLED0);

  ///ei_printf("CamImage img: Image Width      =%d\n", img.getWidth());        //          320
  ///ei_printf("CamImage img: Image Height     =%d\n", img.getHeight());       //          240
  ///ei_printf("CamImage img: Image Size       =%d\n", img.getImgSize());      //      153 600
  ///ei_printf("CamImage img: Image Format     =%d\n", img.getPixFormat());    //1 498 831 189  
  ///ei_printf("CamImage img: Image Buffer Size=%d\n", img.getImgBuffSize());  //      153 600
  ///ei_printf("CamImage img: Image Avaliable  =%d\n", img.isAvailable());     //            1
  
  if (!img.isAvailable())
  {
    ei_printf("In CamCB(): Image is no longer ready...\n");
    return; // Fast path if image is no longer ready.
  } 
  CamErr err;
  ei_printf("In CamCB(): New frame processing...\n");
  
  //
  // Resize and convert image to grayscale to prepare for inferencing
  //
    
  // Clip and resize Image with HW 2D accelerator.
  //    Clip and resize the image with 2D accelerator HW in CXD5602.
  //    First, clip the area specified by the arguments
  //       (#lefttop_x, #lefttop_y) - (#rightbottom_x, # rightbottom_y)
  //       for the original image and
  //    Specify the clipped image with arguments
  //       (#width, # height) resize to the size you made.
  //    The resized image is stored in the CamImage instance specified as the first argument
  //    with new image buffer created internally.
  //    If any error occured such as zero size case, this returns error code.
  //    This HW accelerator has limitation for resizing as below:
  //       Minimum width and height is 12 pixels.
  //       Maximum width  is  768 pixels.
  //       Maximum height is 1024 pixels.
  //       Resizing magnification is 2^n or 1/2^n, and resized image size must be integer.
  err = img.clipAndResizeImageByHW(sized_img,                   // CamImage &img,
                                   OFFSET_X,                    // int 	     lefttop_x,
                                   OFFSET_Y,                    // int 	     lefttop_y,
                                   OFFSET_X + CLIP_WIDTH - 1,   // int 	     rightbottom_x,
                                   OFFSET_Y + CLIP_HEIGHT - 1,  // int 	     rightbottom_y,
                                   EI_CLASSIFIER_INPUT_WIDTH,   // int       width.  Must be the same as the Impulse Input Block.
                                   EI_CLASSIFIER_INPUT_HEIGHT); // int       height. Must be the same as the Impulse Input Block.
  if (err)
  {
    ei_printf("ERR: New frame processing failed. See clipAndResizeImageByHW()...\n");      
    printCamErr(err);
  }
  //ei_printf("In CamCB(): Convert format: ");
  
  // Output still picture format: JPEG(4:2:2), Y/Cb/Cr, YUV, RGB, RAW, JPEG+YUV(thumbnail)
  // Still data rate: 5M pixel 15 frame/s JPEG output
  // CAM_IMAGE_PIX_FMT_RGB565 = V4L2_PIX_FMT_RGB565, /**< RGB565 format */
  // CAM_IMAGE_PIX_FMT_YUV422 = V4L2_PIX_FMT_UYVY,   /**< YUV422 packed. */
  // CAM_IMAGE_PIX_FMT_JPG    = V4L2_PIX_FMT_JPEG,   /**< JPEG format */
  // CAM_IMAGE_PIX_FMT_GRAY,                         /**< Gray-scale */
  // CAM_IMAGE_PIX_FMT_NONE,                         /**< No defined format */
  
  #if GRAYSCALE == true

  //ei_printf("Grayscale Capture\n");
  err = sized_img.convertPixFormat(CAM_IMAGE_PIX_FMT_GRAY);
  if (err)
  {   
    ei_printf("ERR: Converting processing failed. See convertPixFormat(CAM_IMAGE_PIX_FMT_GRAY)...\n");      
  }

  #else
  
  //ei_printf("RGB565 Capture\n");
  err = sized_img.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);
  if (err)
  {   
    ei_printf("ERR: Converting processing failed. See convertPixFormat(CCAM_IMAGE_PIX_FMT_RGB565)...\n");      
  }

  #endif
  if (err)
  {    
    printCamErr(err);
  }

  // Get inference results on resized grayscale image.
  //ei_printf("In CamCB(): Classify picture:\n");
  StateLED1 = !StateLED1;
  digitalWrite(LED1, StateLED1);
  
  // +-------------------------------------------+
  // | Classify the captured image.              |
  // +-------------------------------------------+
  camera_classify(DEBUG_IT);

  // Print the Predictions.
  for (int ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
  {
    ei_printf("\t",ei_classifier_inferencing_categories[ix]);
    ei_printf("\t",ei_result.classification[ix].value);

    // See if we classified an image with high confidence and take action.
    if (ei_classifier_inferencing_categories[ix] != "Unknown")
    {
      if (ei_result.classification[ix].value >= CLASSIFIER_THRESHOLD)
      { 
        digitalWrite(LED2, HIGH);
        //delay(3);                         // TODO: Do something here like save a snapshot to the SD card or
                                          //       turn on a strobe light to scare deer out of the garden, etc.
        digitalWrite(LED2, LOW);
      }
    }
  }
}

/*******************************************************************************************************************
 * @brief      Initialize the camera for continuous monitoring of video feed.
 * @details    
 * @param[in]  doPrintCamErr, To show the camera errors or not.
 * @return     void
 *******************************************************************************************************************/
void camera_start_continuous(bool doPrintCamErr)
{
  //ei_printf("[FLOW] Entering camera_start_continuous()...\n");
  
  // Start the camera.
  CamErr err;
  //ei_printf("Starting the camera:\n");

  // The begin() method function is the first function to call when using "theCamera".
  // Start "theCamera" with
  //    Frame Rate = 5 images per second
  //    Image data pixel format = YUV 422
  // YUV 422 is a YUV model that defines one luminance component (Y) meaning physical linear-space brightness,
  // and two chrominance components, called U (blue projection) and V (red projection) respectively, aka YCbCr.
  err = theCamera.begin(1, CAM_VIDEO_FPS_5,             // Defined in "camera.h".
                           RAW_WIDTH,                   // Defined in this file.
                           RAW_HEIGHT,                  // Defined in this file.
                           CAM_IMAGE_PIX_FMT_YUV422);   // Defined in "camera.h".
  if (err && doPrintCamErr)
  {
    ei_printf("ERR: Starting the camera failed. See theCamera.begin() in camera_start_continuous()...\n");      
    printCamErr(err);
  }

  // Start streaming the Preview images to the Classifier().
  // The viewfinder of a camera shows real-time images shown on a camera.
  // This real-time image (real-time movie) is called Preview image.
  // "theCamera" has a function to acquire this Preview image frame by frame.
  //
  // "startStreaming()" registers the callback function, CamCB(), to obtain the Preview image.
  // CamCB() is a user function.
  // When "true" is specified as the first argument of startStreaming(),
  //    acquisition of the video image for Preview is started, and
  //    the registered callback function is called each time the image is acquired.
  //    The frequency of acquiring images is determined by the frame rate specified by the begin() method function.
  //    The callback function of the next frame will not be called unless the callback function implemented by the user is terminated.
  //    To stop the acquisition of the Preview image, call the startStreaming() method function
  //    with the first argument of the startStreaming() method function set to "false".
  //ei_printf("Starting sending data:\n");
  err = theCamera.startStreaming(true, CamCB);
  if (err && doPrintCamErr)
  {
    ei_printf("ERR: Start sending the data failed. See theCamera.startStreaming()...\n");      
    printCamErr(err);
  }

  /* Auto white balance configuration */
  Serial.println("Set Auto white balance parameter...");
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_DAYLIGHT);
  if (err != CAM_ERR_SUCCESS)
  {
    printCamErr(err);
  }
  
  /*
  // We are not taking snapshots but this is how to set it up.
  // Still image format must be JPEG to allow for compressed storage/transmit.
  //ei_printf("Set format:\n");
  err = theCamera.setStillPictureImageFormat( RAW_WIDTH,
                                              RAW_HEIGHT,
                                              CAM_IMAGE_PIX_FMT_YUV422);
                                            //CAM_IMAGE_PIX_FMT_JPG );
                                            //CAM_IMAGE_PIX_FMT_GRAY);
                                            //CAM_IMAGE_PIX_FMT_RGB565);
  if (err && doPrintCamErr)
  {
    ei_printf("ERR: Setting the image format failed. See theCamera.setStillPictureImageFormat())...\n");      
    printCamErr(err);
  }
  */

  if (doPrintCamErr)
  {
    ei_printf("INFO: Started camera recording...\n");
  }
}

/*******************************************************************************************************************
 * @brief   Arduino setup function
 * @details 
 * @param   void
 * @return  void
 *******************************************************************************************************************/
void setup()
{
  // Setup on-board LEDs.
  pinMode(LED0, OUTPUT);  // Toggles when entering    CamCB().
  pinMode(LED1, OUTPUT);  // Toggles before calling   camera_classify().
  pinMode(LED2, OUTPUT);  // Toggles when we classify an image with high confidence.
  pinMode(LED3, OUTPUT);  // Toggles when entering    ei_camera_cutout_get_data().
  
  Serial.begin(115200);

  // join i2c bus
  Wire.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is the address for oled
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Since we can't clear the Serial Monitor on program startup,
  // we print a demark.
  ei_printf("... ... ... ... ....\n");
  ei_printf("... ... ... ... ....\n");
  ei_printf("... ... ... ... ....\n");
  ei_printf("Spresense Vision Model Inferencing starting up...\n");

  // Print what INO file we are running.
  ei_printf(__FILE__ " " __DATE__ " " __TIME__);
  ei_printf("  IDE "); ei_printf(ARDUINO); ei_printf("\n");

  // Summary of inferencing settings (from model_metadata.h)
  ei_printf("Inferencing settings:\n");
  ei_printf("\tImage resolution: %dx%d\n", EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);  // Stored in "model_metadata.h" = 96x96 // Depends on you Impulse. Go larger if the Model will fit in memory.
  ei_printf("\tFrame size: %d\n",          EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);                     // Stored in "model_metadata.h" = 9216
  ei_printf("\tImage frame size %d\n",     EI_CLASSIFIER_NN_INPUT_FRAME_SIZE);                      // Stored in "model_metadata.h" = 9216
  ei_printf("\tImage type: %d\n",          EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME);                    // Stored in "model_metadata.h" = `
  ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));  // Stored in "model_variables.h" = 3 (depends on classes in your model)
  ei_printf("\tNo. of labels: %d\n",              EI_CLASSIFIER_LABEL_COUNT);                                                           // Stored in "model_metadata.h"  = 3 (depends on classes in your model)
    
  for (int ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)                                            // Stored in "model_metadata.h" = 
  {
    ei_printf("\t\tClass %i: %s\r\n", ix, ei_classifier_inferencing_categories[ix]);                // <labels> of your classes.
  }
  
  ei_printf("\tRaw Image Width: %d Height: %d\n", RAW_WIDTH, RAW_HEIGHT);                           // Stored in this file.         = Raw Image Width: 320 Height: 240
  ei_printf("\tClip Width:      %d Height: %d\n", CLIP_WIDTH, CLIP_HEIGHT);                         // Stored in this file.         = Clip Width: 96 Height: 96
  ei_printf("\tOffset X:        %d Y:      %d\n", OFFSET_X, OFFSET_Y);                              // Stored in this file.         = Offset X: 112 Y: 72
    
  #if defined(CMSIS_NN)
    // CMSIS NN software library, a collection of efficient neural network kernels developed
    // to maximize the performance and minimize the memory footprint of neural networks on Cortex-M processor cores.
    ei_printf("Enabled CMSIS_NN\n"); 
  #endif
    
  #if defined(EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN )
    ei_printf("Enabled EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN : %d\n", EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN ); 
  #endif

  // Kick-off the inferencing loop.
  camera_start_continuous(DEBUG_IT); 
}

/*******************************************************************************************************************
 * @brief   Arduino main function.
 * @details 
 * @param   void
 * @return  void
*******************************************************************************************************************/
void loop()
{
  //ei_printf("In loop()...\n");
  
  //sleep(100);
}
/* Flowchart ---------------------------------------------------------------------------------------------------- */
/*
            setup()
              camera_start_continuous()                          

            loop()
              Entering CamCB()
                
              Entering camera_classify()
              Entering RGB565, ei_camera_cutout_get_data()
                . . .                
              Entering RGB565, ei_camera_cutout_get_data()

                . . .

              Entering camera_classify()
              Entering RGB565, ei_camera_cutout_get_data()
                . . .                
              Entering RGB565, ei_camera_cutout_get_data()
*/  