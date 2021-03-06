/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2014-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <thread>
#include <vector>

#include "ErrorBase.h"
#include "HAL/cpp/priority_mutex.h"
#include "Vision/ColorImage.h"
#include "Vision/HSLImage.h"
#include "nivision.h"

namespace nivision {

/**
 * Axis M1011 network camera
 */
class AxisCamera : public frc::ErrorBase {
 public:
  enum WhiteBalance {
    kWhiteBalance_Automatic,
    kWhiteBalance_Hold,
    kWhiteBalance_FixedOutdoor1,
    kWhiteBalance_FixedOutdoor2,
    kWhiteBalance_FixedIndoor,
    kWhiteBalance_FixedFluorescent1,
    kWhiteBalance_FixedFluorescent2
  };

  enum ExposureControl {
    kExposureControl_Automatic,
    kExposureControl_Hold,
    kExposureControl_FlickerFree50Hz,
    kExposureControl_FlickerFree60Hz
  };

  enum Resolution {
    kResolution_640x480,
    kResolution_480x360,
    kResolution_320x240,
    kResolution_240x180,
    kResolution_176x144,
    kResolution_160x120,
  };

  enum Rotation { kRotation_0, kRotation_180 };

  explicit AxisCamera(std::string const& cameraHost);
  virtual ~AxisCamera();

  AxisCamera(const AxisCamera&) = delete;
  AxisCamera& operator=(const AxisCamera&) = delete;

  bool IsFreshImage() const;

  int GetImage(Image* image);
  int GetImage(ColorImage* image);
  HSLImage* GetImage();
  int CopyJPEG(char** destImage, int& destImageSize, int& destImageBufferSize);

  void WriteBrightness(int brightness);
  int GetBrightness();

  void WriteWhiteBalance(WhiteBalance whiteBalance);
  WhiteBalance GetWhiteBalance();

  void WriteColorLevel(int colorLevel);
  int GetColorLevel();

  void WriteExposureControl(ExposureControl exposureControl);
  ExposureControl GetExposureControl();

  void WriteExposurePriority(int exposurePriority);
  int GetExposurePriority();

  void WriteMaxFPS(int maxFPS);
  int GetMaxFPS();

  void WriteResolution(Resolution resolution);
  Resolution GetResolution();

  void WriteCompression(int compression);
  int GetCompression();

  void WriteRotation(Rotation rotation);
  Rotation GetRotation();

 private:
  std::thread m_captureThread;
  std::string m_cameraHost;
  int m_cameraSocket = -1;
  priority_mutex m_captureMutex;

  priority_mutex m_imageDataMutex;
  std::vector<uint8_t> m_imageData;
  bool m_freshImage = false;

  int m_brightness = 50;
  WhiteBalance m_whiteBalance = kWhiteBalance_Automatic;
  int m_colorLevel = 50;
  ExposureControl m_exposureControl = kExposureControl_Automatic;
  int m_exposurePriority = 50;
  int m_maxFPS = 0;
  Resolution m_resolution = kResolution_640x480;
  int m_compression = 50;
  Rotation m_rotation = kRotation_0;
  bool m_parametersDirty = true;
  bool m_streamDirty = true;
  priority_mutex m_parametersMutex;

  bool m_done = false;

  void Capture();
  void ReadImagesFromCamera();
  bool WriteParameters();

  int CreateCameraSocket(std::string const& requestString, bool setError);
};
}  // namespace nivision
