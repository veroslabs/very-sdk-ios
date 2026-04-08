// Copyright (c) 2020 RedRock Biometrics. All rights reserved.
#ifndef __PALM_ID_H__
#define __PALM_ID_H__

#ifndef PALM_EXPORT
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define PALM_EXPORT EMSCRIPTEN_KEEPALIVE
#elif _MSC_VER
#define PALM_EXPORT __declspec(dllexport)
#else
#define PALM_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef PALM_CALL
#ifdef _MSC_VER
#define PALM_CALL __stdcall
#else
#define PALM_CALL
#endif
#endif

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Possible return values from the various API functions.
 */
typedef enum _ePalmStatus {
  /**
   * The desired operation completed successfully.
   */
  ePalm_Success = 0,

  /**
   * The function timed-out before completing the desired operation.
   */
  ePalm_Timeout = 1,

  /**
  * Some allocated memory was not freed.
  */
  ePalm_MemoryLeak = 2,

  /**
   * There was an error completing the desired operation, but for an unknown
   * reasons.
   */
  ePalm_UnknownError = 0x8000,

  /**
   * An operation was requested when it was not expected.
   */
  ePalm_InvalidHandle = 0x8001,

  /**
   * One of the provided arguments was invalid or incomplete.
   */
  ePalm_InvalidArgument = 0x8002,

  /**
   * Insufficient memory in buffer.
   */
  ePalm_OutOfMemory = 0x8003,

  /**
   * An operation was requested when it was not expected.
   */
  ePalm_UnexpectedRequest = 0x8004,

  /**
   * The license id used was invalid.
   */
  ePalm_InvalidLicense = 0x8005,

  /**
   * The model does not exist or is invalid.
   */
  ePalm_InvalidModel = 0x8006,

  /**
   * Error connecting to API server.
   */
  ePalm_ServerConnectionError = 0x8007,

  /**
   * Error in data serialization.
   */
  ePalm_SerializationError = 0x8008,

  /**
   * Error in palm model creation due to palm not present.
   */
  ePalm_PalmNotPresent = 0x8009,

  /**
   * Error in palm model creation due to palm brightness not ideal.
   */
  ePalm_PalmBrightness = 0x8010,

  /**
   * Error in palm model creation due to palm size too big or small.
   */
  ePalm_PalmSize = 0x8011,

  /**
   * Error in palm model creation due to palm orientation.
   */
  ePalm_PalmOrientation = 0x8012,

  /**
   * Error in palm model creation due to invalid palm position.
   */
  ePalm_PalmPosition = 0x8013,

  /**
   * Error in palm model creation due to invalid palm gesture such as palm not open.
   */
  ePalm_PalmGesture = 0x8014,

  /**
   * Error in palm model creation due to invalid palm frame image.
   */
  ePalm_InvalidFrame = 0x8015
} ePalmStatus;

/**
* The type of palm to use.
*/
typedef enum _PalmType {
  /**
  * A visible capture of the palm print.
  */
  PalmType_Print = 0,

  /**
  * An infrared capture of the palm veins.
  */
  PalmType_Vein = 1
} PalmType;

/**
* The type of palm to use.
*/
typedef enum _PalmAdjustment {
  /**
  * Palm passes all quality checks, no adjustment is needed.
  */
  PalmAdjustment_None = 0,

  /**
  * The palm needs to move to the left (in the image space).
  */
  PalmAdjustment_MoveLeft = 1,

  /**
  * The palm needs to move to the right (in the image space).
  */
  PalmAdjustment_MoveRight = 2,

  /**
  * The palm needs to move up (in the image space).
  */
  PalmAdjustment_MoveUp = 3,

  /**
  * The palm needs to move down (in the image space).
  */
  PalmAdjustment_MoveDown = 4,

  /**
  * The palm needs to move closer to the camera.
  */
  PalmAdjustment_MoveCloser = 5,

  /**
  * The palm needs to move farther away from the camera.
  */
  PalmAdjustment_MoveFarther = 6,

  /**
  * Palm needs to be rotated to keep the fingers upwards in the camera image.
  */
  PalmAdjustment_RotateUpward = 7,

  /**
  * Palm needs to be rotated parallel to the camera.
  */
  PalmAdjustment_RotateParallel = 8,

  /**
  * The back of the palm is shown and needs to be flipped over.
  */
  PalmAdjustment_FlipOver = 9,

  /**
  * Fingers need to spread farther apart.
  */
  PalmAdjustment_SpreadFingers = 10,

  /**
  * Fingers need to be straightened.
  */
  PalmAdjustment_StraightenFingers = 11,

  /**
  * Thumb is blocking the palm and needs to be moved.
  */
  PalmAdjustment_SpreadThumb = 12,

  /**
  * Fingers need to be straightened.
  */
  PalmAdjustment_StraightenThumb = 13,

  /**
  * Palm is too dark and needs more light to capture.
  */
  PalmAdjustment_Brighten = 20,

  /**
  * Palm is over-exposed and light must be reduced to capture.
  */
  PalmAdjustment_Darken = 21,

  /**
  * The palm image is too blurry and needs to be focused.
  */
  PalmAdjustment_Focus = 22,

  /**
  * The palm is moving too quickly to capture.
  */
  PalmAdjustment_KeepStill = 23,

  /**
  * No palm is detected.
  */
  PalmAdjustment_No_Palm = 99,

  /**
  * Palm fails for an unknown reason.
  */
  PalmAdjustment_Other = 100,
} PalmAdjustment;

/**
* The status of the flash light source.
*/
typedef enum _PalmFlash {
  /**
  * Unable to query the flash status.
  */
  PalmFlash_Invalid = -1,

  /**
  * Flash should be turned off.
  */
  PalmFlash_Off = 0,

  /**
  * Flash should be turned on.
  */
  PalmFlash_On = 1,
} PalmFlash;

typedef struct _PalmImage {
  /**
   * Type of the image.
   */
  PalmType type;

  /**
   * Pointer to the raw image data.
   */
  uint8_t* data;

  /**
   * Bit depth of the image in bits.
   */
  uint32_t depth;

  /**
   * Width of the image in pixels.
   */
  uint32_t width;

  /**
   * Height of the image in pixels.
   */
  uint32_t height;

  /**
   * Size of a row of pixels in bytes, including any padding at end.
   */
  uint32_t stride;

  /**
   * Number of bytes offset from the beginning of data to the start of image.
   */
  uint32_t offset;

  /**
   * The time at which the frame was captured in microseconds.
   */
  int64_t timestamp;

  /**
   * If non-zero, the capturing device has active illumination such as infrared LEDs or torch.
   */
  int8_t active_lighting;

  /**
   * If non-zero, the camera is adjusting focus or exposure when capturing this frame.
   */
  int8_t camera_under_adjustment;

  /**
   * If non-zero, the camera is facing away from the user.
   */
  int8_t rear_facing_camera;
} PalmImage;

/**
 * A camera device frame.
 */
typedef struct _PalmFrame {
  /**
  * Number of images associated with this frame.
  */
  uint32_t num_images;

  /**
   * Pointers to images associated with this frame. Support up to 8 images but only the first 'num_images' are allocated and used.
   */
  PalmImage* images[8];
} PalmFrame;

typedef enum _PalmSidedness {
  /**
  * The handedness could not be determined
  */
  PalmSidedness_Undetermined = -1,

  /**
  * The palm is a left hand
  */
  PalmSidedness_Left = 0,

  /**
  * The palm is a right hand
  */
  PalmSidedness_Right = 1
} PalmSidedness;

/**
* Strictness refers to how accurately the palm needs to be aligned to the guide, as well as other criteria,
* in order to capture it.  A higher strictness means more accurate matching, but may be more difficult for
* a user to position their hand for capture.  A lower strictness means a palm can be placed in more places
* within the camera's field of view, but matching accuracy may be lower.
*/
typedef enum _PalmStrictness {
  /**
  * The palm cannot be captured.
  */
  PalmStrictness_None = -1,

  /**
  * The minimum possible strictness to capture a palm. (1/5)
  */
  PalmStrictness_Minimum = 0,

  /**
  * A high strictness criterion recommended for palm authentication captures. (2/5)
  */
  PalmStrictness_Authenticate = 1,

  /**
  * A medium strictness to capture a palm. (3/5)
  */
  PalmStrictness_Medium = 2,

  /**
  * A high strictness requirement recommended for palm enrollment captures. (4/5)
  */
  PalmStrictness_Enrollment = 3,
  PalmStrictness_Enrollment_Unique = 4,

  /**
  * The maximum possible strictness to capture a palm. (5/5)
  */
  PalmStrictness_Maximum = 5
} PalmStrictness;

typedef enum _PalmLiveness {
  /**
  * No liveness is preformed.
  */
  PalmLiveness_None = 0,

  /**
  * After palm capture, user is prompted to make a gesture.
  */
  PalmLiveness_Gesture = 1,

  /**
  * After palm capture, a high-resolution image with active lighting is captured.
  */
  PalmLiveness_HiRes = 2,

  /**
  * Combination of PalmLiveness_HiRes followed by PalmLiveness_Gesture.
  */
  PalmLiveness_HiRes_Gesture = 3,

  /**
  * After palm capture, user is prompted to touch the thumb tip with a joint on another finger.
  */
  PalmLiveness_Touch = 4,

  /**
  * After palm capture, active lighting is flashed on and off with further captures.
  */
  PalmLiveness_Flash = 5,

  /**
  * Combination of PalmLiveness_Flash followed by PalmLiveness_Gesture.
  */
  PalmLiveness_Flash_Gesture = 6,

  /**
  * Combination of PalmLiveness_HiRes followed by PalmLiveness_Flash.
  */
  PalmLiveness_HiRes_Flash = 7,

  /**
  * Combination of PalmLiveness_HiRes followed by PalmLiveness_Flash followed by PalmLiveness_Gesture.
  */
  PalmLiveness_HiRes_Flash_Gesture = 8
} PalmLiveness;

typedef enum _PalmGesture {
  /**
  * No gesture
  */
  PalmGesture_None = 0,

  /**
  * Pointing index finger
  */
  PalmGesture_Pointing = 1,

  /**
  * Pointing index and middle fingers
  */
  PalmGesture_Scissors = 2,

  /**
  * Number three gesture
  */
  PalmGesture_Three = 3,

  /**
  * Number four gesture
  */
  PalmGesture_Four = 4,

  /**
  * Fist
  */
  PalmGesture_Fist = 6,

  /**
  * Pointing only thumb
  */
  PalmGesture_Thumbs_Up = 7,

  /**
  * Pointing thumb and index fingers
  */
  PalmGesture_L = 8,

  /**
  * All fingers flattened together
  */
  PalmGesture_Paper = 9,

  /**
  * Thumb and index touching in a circle
  */
  PalmGesture_Circle = 10,

  /**
  * Palm turned backwards
  */
  PalmGesture_Back = 11,

  /**
  * Thumb and pinky out, other fingers curled
  */
  PalmGesture_Shaka = 12,

  /**
  * Pointing thumb, index, and middle fingers
  */
  PalmGesture_Double_L = 13,
} PalmGesture;

typedef struct _PalmModelID {
  /**
   * Unique identifier for palm model
   */
  uint8_t id[20];
} PalmModelID;

typedef struct _PalmNonce {
  /**
   * Nonce for palm model request. Default to all 0.
   */
  uint8_t nonce[16];
} PalmNonce;

/**
 * A 2-D point.
 */
typedef struct _PalmPoint2D {
  float x;
  float y;
} PalmPoint2D;

/**
 * A 2-D quadrilateral described by four 2-D points.
 */
typedef struct _PalmQuad2D {
  PalmPoint2D a;
  PalmPoint2D b;
  PalmPoint2D c;
  PalmPoint2D d;
} PalmQuad2D;

typedef struct _PalmMetrics {
  /**
   * A quadrilateral in image coordinates enclosing the palm.
   */
  PalmQuad2D quad;

  /**
   * X coordinate of the center of the palm divided by the shorter side of the image.
   */
  float center_x;

  /**
   * y coordinate of the center of the palm divided by the shorter side of the image.
   */
  float center_y;

  /**
   * Radius of the palm area divided by the shorter side of the image.
   */
  float palm_radius;

  /**
   * Measured spread angle between the 4 fingers in degrees.
   */
  float finger_spread_angle;

  /**
   * Measured bending angle between the 4 fingers in degrees.
   */
  float finger_bend_angle;

  /**
   * Measured spread angle between thumb and the palm in degrees.
   */
  float thumb_spread_angle;

  /**
   * Measured bending angle between the thumb and the palm in degrees.
   */
  float thumb_bend_angle;

  /**
   * Rotation angle in degrees of the axis going through the top of the palm.
   */
  float palm_angle_dorsal;

  /**
   * Rotation angle in degrees of the axis going out from the fingers of the palm.
   */
  float palm_angle_distal;

  /**
   * Rotation angle in degrees of the palm tilting up and down.
   */
  float palm_angle_radial;

  /**
   * A scaler between -1 and 1 to indicate the brightness of the palm image (0 is good, -1 is too dark, and 1 is too bright)
   */
  float brightness;

  /**
   * A scaler between 0 and 1 to indicate whether the palm is overexposed (0 is not overexposed and 1 is overexposed)
   */
  float overexposure;

  /**
   * A scaler between 0 and 1 to indicate the bluriness of the palm image (0 is sharp and 1 is blurry)
   */
  float blurriness;

  /**
   * A scaler between 0 and 1 to indicate the stillness of the palm image (0 means palm is still and 1 means palm moves too fast)
   */
  float motion;

  /**
   * A scaler between 0 and 1 to indicate the likelihood of the detected palm being flipped over (0 is definitely not flipped and 1 is definitely flipped).
   */
  float flipness;

  /**
   * A scaler between 0 and 1 to indicate the likelihood of the detected palm being left or right (0 is definitely left and 1 is definitely right).
   */
  float sidedness;

} PalmMetrics;

typedef struct _PalmDetected {
  /**
   * A set of parameters indicating different palm measurements.
   */
  PalmMetrics metrics;

  /**
   * Adjustment that needs to be made to capture a palm.
   */
  PalmAdjustment adjustment;

  /**
   * The highest strictness level that the palm could be immediately captured.
   * If no palm can be captured, this value will be PalmStrictness_None.
   */
  PalmStrictness readiness;

} PalmDetected;

/**
 * The palms detected in the latest frame.
 */
typedef struct _PalmsDetected {
  /**
   * The number of palms detected.
   */
  uint32_t num_palms;

  /**
   * Timestamp of when image with detected palm was captured in microseconds.
   */
  int64_t timestamp;

  /**
   * An array of detected palms, consisting of num_detected palms.
   */
  PalmDetected* palms;
} PalmsDetected;

/**
 * The capture result.
 */
typedef struct _PalmCaptureResult {
  /**
   * A unique identifier for the captured palm
   */
  PalmModelID model_id;

  /**
   * A flag to indicate if palm model is for left or right.
   */
  PalmSidedness sidedness;

  /**
   * A bitcode to indicate palm type. 0-Unknown, 1-Print, 2-Vein, 3-Print and Vein
   */
  uint8_t type;

  /**
   * A pointer to the data stream for this model. This will be null if capture failed.
   */
  void* data;

  /**
  * The size of the model data in bytes. This will be 0 if capture failed.
  */
  uint32_t data_size;
} PalmCaptureResult;

typedef enum _PalmLivenessStatus {
  /**
  * It could not be determined if the detected palm is a live one or not.
  */
  PalmLivenessStatus_Undetermined = -1,

  /**
  * The palm passes the liveness check.
  */
  PalmLivenessStatus_Pass = 0,

  /**
  * The palm fails the liveness check.
  */
  PalmLivenessStatus_Fail = 1,

  /**
  * The palm fails because the background is too bright.
  */
  PalmLivenessStatus_Fail_Background = 2,

  /**
  * The palm fails because it disappears.
  */
  PalmLivenessStatus_Fail_LostHand = 3,

  /**
  * The palm fails because the background is too bright, waiting for another attempt.
  */
  PalmLivenessStatus_Fail_Background_Continue = 4,

  /**
  * The palm fails the liveness check, waiting for another attempt.
  */
  PalmLivenessStatus_Fail_Continue = 5
} PalmLivenessStatus;

/**
* The liveness check result.
*/
typedef struct _PalmLivenessResult {
  /**
  * The liveness check status;
  */
  PalmLivenessStatus status;

  /**
  * The model id of the palm
  */
  PalmModelID model_id;
} PalmLivenessResult;

/**
 * Set configuration parameters. A parameter is specified by two strings [name, value].
 * Call this function before calling PalmID_Create.
 *
 * @param key A pointer to parameter name string. The string is expected to end with '\0'.
 * @param value A pointer to parameter value string. The string is expected to end with '\0'.
 *
 * For possible key values, see 'PalmID_GetConfig'.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_SetConfig(const char* key, const char* value);

/**
 * Get configuration parameters.
 *
 * @param key A pointer to parameter name string. The string is expected to end with '\0'.
 * @param value A pointer to hold the output parameter value string. The caller needs to allocate the memory.
 * @param value_size A pointer to an integer to hold the output parameter value string size. This parameter
 * is updated upon return with the actual number of bytes written to the value pointer.
 * @param null_terminate A boolean value on whether or not to include a null-termination character ('\0') at
 * the end of the value string (0 is no, otherwise yes).
 *
 * @note: If the size of the value buffer is insufficient to hold the entire value, this function will
 * return an ePalm_InsufficientMemory error, and the value_size parameter will be updated with the actual
 * size needed to store the results.
 *
 * Possible keys include:
 *   "version":         The full version number of the SDK and build (read-only).
 *   "major_version":   The major version number of the SDK (read-only).
 *   "minor_version":   The minor version number of the SDK (read-only).
 *   "license_id":      The license id required for authentication.
 *   "rotation_angle":  Rotation of the palm in degrees.  Fingers pointing up in the image is 0 degrees
 *                      and increases clockwise to 360 degrees. Currently we only support 0, 90, 180, 270.
 *                      The value "auto" may also be used so the SDK can automatically choose the angle.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_GetConfig(const char* key, char* value, int32_t* value_size, int null_terminate);

/**
 * Current time in microseconds.
 *
 * @return The current time in microseconds. A negative value indicates failure.
 */
PALM_EXPORT int64_t PALM_CALL PalmID_Now(void);

/**
* Insert a palm image to a palm frame. This function creates a new palm image of specified type and size and allocates memory
* for its image data buffer. If the frame already contains the maximum number of images, this function does nothing.
*
* @param frame   A palm frame to add the new image.
*        width   Image width
*        height  Image height
*        depth   Image depth in bits.  Must be 8 (Grayscale) or 24 (RGB)
*        channel Image channel source
*/
PALM_EXPORT void PALM_CALL PalmFrame_AddImage(PalmFrame* frame, uint32_t width, uint32_t height, uint32_t depth, PalmType type);

/**
* Insert a palm image to a palm frame. This function creates a new palm image from a binary file such as a BMP, PNG, or JPG
* and allocates memory for its image data buffer. If the frame already contains the maximum number of images, this function does nothing.
*
* @param frame     A palm frame to add the new image.
*        file_data The binary file data
*        file_size Size of the file data
*        channel   Image channel source
*/
PALM_EXPORT void PALM_CALL PalmFrame_AddImageFromFile(PalmFrame* frame, const void* file_data, uint32_t file_size, PalmType type);

/**
* Insert a palm image to a palm frame. This function creates a new palm image from an encoded image data stream such as YUV, converts into RGB24,
* and allocate memory for its image data buffer. If the frame already contains the maximum number of images, this function does nothing.
*
* @param frame     A palm frame to add the new image.
*        data    Pointer to the binary data stream
*        width   Image width
*        height  Image height
*        row_padding  Equals rawstride - width * pixelsize, usually zero
*        encode  Type of encoding. 0--YUV420; 1--NV12; 2--NV21; 3--RGBA; 4--BGRA; 5--JPEG
* @note when input is jpeg, set width to the length of jpeg stream and height to 1.
*/
PALM_EXPORT void PALM_CALL PalmFrame_AddEncodedImage(PalmFrame* frame, void* data, uint32_t width, uint32_t height, uint32_t row_padding, uint8_t encode, PalmType type);

/**
* Create an empty palm frame with no images.
*
* @param frame      A pointer to a palm frame that will be initialized as an empty frame with no images.

* @note call PalmFrame_AddImage to insert an image into a frame. Note that the memory for the palm frame and images are allocated
*       inside the PalmAPI shared library.
*/
PALM_EXPORT void PALM_CALL PalmFrame_Create(PalmFrame* frame);

/**
* Free a palm frame and its images.
*
* @param frame A palm frame allocated inside Palm API shared library.
*
* @note This function releases the memories allocated for a frame and all its images.
*/
PALM_EXPORT void PALM_CALL PalmFrame_Free(PalmFrame* frame);

#ifdef __cplusplus
}
#endif

#endif /* __PALM_ID_H__ */
