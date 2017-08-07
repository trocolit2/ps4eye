#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>

enum InputModeEnum {
  SAVED_IMAGES = 0,
  REAL_TIME_2_CAMERAS = 1,
  SAVED_CONCATED_IMAGES = 2,
  REAL_TIME_CAMERA_CONCATED_IMAGES = 3,
};
typedef InputModeEnum InputMode;

struct InputParamatersStruct {
  cv::Size board_size;
  int pair_images_number;
  float square_size;
  int cam_device_1;
  int cam_device_2;

  std::string prefix_left;
  std::string prefix_right;
  std::string path_images;
  std::string sufix;

  InputMode input_mode = (InputMode) 0 ;

};
typedef InputParamatersStruct InputParameters;


const std::string keys =
  "{h    |  board_height|            6|Height of the board}"
  "{w    |    board_with|            9|Width of the board}"
  "{s    |   square_size|        0.035|Board Square Size in meter}"
  "{im   |    input_mode|            0|Clicks stereo images before calibration.\
                                       Use if you do not have stereo pair images \
                                       saved}"
  "{n    | number_images|           30|Number of stereo pair images}"
  "{dr   |     directory|            .|Directory of images}"
  "{prel |   prefix_left|  image_left_|Left image name prefix. Ex: image_left_}"
  "{prer |  prefix_right| image_right_|Right image name postfix. Ex: image_right_}"
  "{sfx  |         sufix|          jpg|Image extension. Ex: jpg, png etc}"
  "{c1   |  cam_device_1|            0|Camera 1 Index}"
  "{c2   |  cam_device_2|            2|Camera 2 Index}";


void help(cv::CommandLineParser parser){
  parser.printParams();
}

void fillInputParans(cv::CommandLineParser parser,
                     InputParameters &input_params){

  input_params.board_size = cv::Size(parser.get<int>("w"),
                                     parser.get<int>("h"));
  input_params.square_size = parser.get<double>("s");
  input_params.pair_images_number = parser.get<int>("n");
  input_params.prefix_left = parser.get<std::string>("prel");
  input_params.prefix_right = parser.get<std::string>("prer");
  input_params.sufix = parser.get<std::string>("sfx");
  input_params.path_images = parser.get<std::string>("dr");
  input_params.input_mode = (InputMode) parser.get<int>("im");

}

void printParams(InputParameters input_params){

  std::cout << "Input parans :"
            << "\nBoard Size -->  " << input_params.board_size
            << "\nSquare Size -->  " << input_params.square_size
            << "\nNumber of Image Pairs -->  "
              << input_params.pair_images_number
            << "\nPrefix Left -->  " << input_params.prefix_left
            << "\nPrefix Right -->  " << input_params.prefix_right
            << "\nSufix -->  " << input_params.sufix
            << "\nDirectory -->  " << input_params.path_images
            << "\nInput Mode -->  " << input_params.input_mode
            << std::endl;
}


void loadSavedImages(InputParameters input_parameters,
                     std::vector<cv::VideoCapture> &captures ){

  std::string path_left = input_parameters.path_images + "/"
                          + input_parameters.prefix_left + "%02d."
                          + input_parameters.sufix;
  std::string path_right = input_parameters.path_images + "/"
                           + input_parameters.prefix_right + "%02d."
                           + input_parameters.sufix;

  std::cout << "left path " << path_left << std::endl;
  std::cout << "right path " << path_right << std::endl;

  cv::VideoCapture capture_left(path_left);
  cv::VideoCapture capture_right(path_right);

  captures.push_back(capture_left);
  captures.push_back(capture_right);

}


bool findChessboard(cv::Mat image_left, cv::Mat image_right,
                    cv::Size board_size, cv::Mat &out,
                    std::vector<cv::Point2f> &corners_left,
                    std::vector<cv::Point2f> &corners_right){

  cv::Mat image_temp_left = image_left.clone();
  cv::Mat image_temp_right = image_right.clone();

  cvtColor(image_left, image_left, cv::COLOR_BGR2GRAY);
  cvtColor(image_right, image_right, cv::COLOR_BGR2GRAY);

  // image_left = image_left > 128;
  // image_right = image_right > 128;

  bool found_left = false, found_right = false;
  found_left = findChessboardCorners(image_left, board_size, corners_left);
  found_right = findChessboardCorners(image_right, board_size, corners_right);

  drawChessboardCorners(image_temp_left, board_size, corners_left, found_left);
  drawChessboardCorners(image_temp_right, board_size, corners_right, found_right);

  cv::Mat image_local_out;
  cv::hconcat(image_temp_left, image_temp_right, image_local_out);
  image_local_out.copyTo(out);

  std::cout << "STATUS CHESSBOARD DETECTION: LEFT IMAGE ";
  found_left ? std::cout << " FOUND " : std::cout << " NOT FOUND ";
  std::cout << "| RIGHT IMAGE ";
  found_right ? std::cout << " FOUND " : std::cout << " NOT FOUND ";
  std::cout << "." << std::endl;

  return found_right && found_left;
}

std::vector< cv::Mat > calibrateStereoCamera(
                      std::vector< std::vector<cv::Point2f> > *vector_corners,
                      std::vector< std::vector<cv::Point3f> > object_points,
                      cv::Size image_size,
                      std::vector<cv::Rect> &valid_roi){

  // std::cout << "DEBUG " << " image size " << image_size << std::endl;

  cv::Mat camera_matrix[2], distortion_coeffs[2];
  camera_matrix[0] = cv::Mat::eye(3, 3, CV_64F);
  camera_matrix[1] = cv::Mat::eye(3, 3, CV_64F);
  cv::Mat rotation, translation, essential, fundamental;

  double rms = stereoCalibrate(object_points,
                               vector_corners[0], vector_corners[1],
                               camera_matrix[0], distortion_coeffs[0],
                               camera_matrix[1], distortion_coeffs[1],
                               image_size,
                               rotation,
                               translation,
                               essential,
                               fundamental,
                               cv::TermCriteria(CV_TERMCRIT_ITER
                                                  + CV_TERMCRIT_EPS,
                                                100, 1e-6),
                               CV_CALIB_ZERO_TANGENT_DIST
                                 + CV_CALIB_RATIONAL_MODEL);

  std::cout << "\n\tRMS Error = " << rms << std::endl;

  double error = 0;
  int number_points = 0;
  std::vector<cv::Vec3f> lines[2];

  for (int i = 0; i < object_points.size(); i++) {
    double error_per_image = 0;
    int local_number_points = (int) vector_corners[0][i].size();
    cv::Mat img_points[2];

    for (int k = 0; k < 2; k++) {
      img_points[k] = cv::Mat(vector_corners[k][i]);
      cv::undistortPoints(img_points[k], img_points[k], camera_matrix[k],
                      distortion_coeffs[k], cv::Mat(), camera_matrix[k]);

      cv::computeCorrespondEpilines(img_points[k], k + 1,
                                    fundamental, lines[k]);
    }

    for (int j = 0; j < local_number_points; j++) {
      double err_ij = fabs(vector_corners[0][i][j].x * lines[1][j][0]
                           + vector_corners[0][i][j].y * lines[1][j][1]
                           + lines[1][j][2])
                      + fabs(vector_corners[1][i][j].x * lines[0][j][0]
                             + vector_corners[1][i][j].y * lines[0][j][1]
                             + lines[0][j][2]);
      error_per_image += err_ij;
    }
    std::cout << "\tPair ID = " << i << " | Reprojection Error = "
              << error_per_image / local_number_points << std::endl;

    error += error_per_image;
    number_points += local_number_points;

  }

  std::cout << "\tAverage Reprojection Error: "
            << error / number_points << std::endl;


  cv::Mat rectification[2], projection[2], disparity_to_depth;
  cv::Rect local_valid_roi[2];
  cv::stereoRectify(camera_matrix[0], distortion_coeffs[0], camera_matrix[1],
                    distortion_coeffs[1], image_size, rotation, translation,
                    rectification[0], rectification[1],
                    projection[0], projection[1],
                    disparity_to_depth, 0, 1,
                    image_size, &local_valid_roi[0], &local_valid_roi[1]);

  std::vector< cv::Mat > out(13);
  out[0] = camera_matrix[0];
  out[1] = camera_matrix[1];
  out[2] = distortion_coeffs[0];
  out[3] = distortion_coeffs[1];

  out[4] = rotation;
  out[5] = translation;
  out[6] = essential;
  out[7] = fundamental;

  out[8] = rectification[0];
  out[9] = rectification[1];
  out[10] = projection[0];
  out[11] = projection[1];
  out[12] = disparity_to_depth;

  valid_roi.push_back(local_valid_roi[0]);
  valid_roi.push_back(local_valid_roi[1]);

  return out;
}


bool writeYMLfile(std::vector< cv::Mat > mats,
                  std::vector<cv::Rect> valid_roi,
                  std::string file_name = "stereo_calibration.yml",
                  std::string file_path = ""){

  bool writed = true;
  cv::FileStorage fs(file_path+file_name, CV_STORAGE_WRITE);
  if( fs.isOpened() ){
      fs << "K1" << mats[0] // camera_matrix 1
         << "K2" << mats[1] // camera_matrix 2
         << "D1" << mats[2] // distortion_coeffs 1
         << "D2" << mats[3] // distortion_coeffs 2

         << "R"  << mats[4] // rotation relative camera 1
         << "T"  << mats[5] // translation relative camera 1
         << "E"  << mats[6] // essential matrix
         << "F"  << mats[7] // fundamental matrix

         << "R1" << mats[8] // rectification matrix 1
         << "R2" << mats[9] // rectification matrix 2
         << "P1" << mats[10] // projection matrix 1
         << "P2" << mats[11] // projection matrix 2
         << "Q"  << mats[12] // disparity to depth matrix

         << "v1" << valid_roi[0] // valide roi camera 1
         << "v2" << valid_roi[1]; // valide roi camera 2

      fs.release();
  }else{
    writed = false;
    std::cout << "Error: can not save the intrinsic parameters\n";
  }
  return writed;
}


std::vector<cv::VideoCapture> initVideoCapture(
                                          InputParameters input_parameters){
  std::vector<cv::VideoCapture> captures;

  switch (input_parameters.input_mode) {
  case SAVED_IMAGES:
    loadSavedImages(input_parameters, captures);
    break;
  case REAL_TIME_CAMERA_CONCATED_IMAGES:
      // calibrateInRealTime(parser.get<int>("cam1"), parser.get<int>("cam2"));
      break;
  case REAL_TIME_2_CAMERAS:
      // calibrateInRealTime(parser.get<int>("cam1"), parser.get<int>("cam2"));
      break;
  default:
      std::cout << "Input Mode should be 0, 1 or 2. Ex: -im=0\n \
                    Execute without any argument to see help \n";
      break;
  }
  return captures;
}

bool calibrationProcess(
                InputParameters input_parameters,
                std::vector< std::vector<cv::Point2f> > *vector_corners,
                cv::Size image_size,
                std::vector<cv::Rect> &valid_roi,
                std::vector< cv::Mat > &mats){


  //Check number of detected pairs
  int detected_chessboards = vector_corners[0].size();
  if(detected_chessboards < 10){
    std::cout << " Required at least 10 detected chessboard pair. \n";
    std::cout << " Current detected " << detected_chessboards << ". \n";
    return false;
  }

  //generate object point
  std::vector< std::vector<cv::Point3f> > object_points(detected_chessboards);
  for (int i = 0; i < detected_chessboards; i++)
      for (int j = 0; j < input_parameters.board_size.height; j++)
          for (int k = 0; k < input_parameters.board_size.width; k++) {
              object_points[i].push_back(cv::Point3f(
                                      float(k) * input_parameters.square_size,
                                      float(j) * input_parameters.square_size,
                                      0.0));
          }

  // calibration stereo camera
  std::vector<cv::Rect> valid_roi_local;
  std::vector< cv::Mat > mats_local = calibrateStereoCamera( vector_corners,
                                object_points, image_size, valid_roi_local);

  mats = mats_local;
  valid_roi = valid_roi_local;

  // write stere camera calibration matrixs
  bool write_file = false;
  write_file = writeYMLfile(mats, valid_roi);
  std::string write_msg;

  write_file ? write_msg = "YML file writed " :
              write_msg = "YML file did not write ";
  std::cout <<"\t"<< write_msg << std::endl;

  return true;

}


void showRectified( cv::Mat image_1, cv::Mat image_2,
                    std::vector< std::vector <cv::Mat> > remap,
                    cv::Mat canvas,
                    bool is_vertical,
                    cv::Size show_size,
                    double size_factor,
                    std::vector<cv::Rect> valid_rois){

  std::vector<cv::Mat> images(2);
  images[0] = image_1;
  images[1] = image_2;

  for( int k = 0; k < 2; k++ ){
      cv::Mat img = images[k], rimg, cimg;
      cv::remap(img, rimg, remap[k][0], remap[k][1], CV_INTER_LINEAR);
      if(img.channels() < 3)
        cv::cvtColor(rimg, cimg, cv::COLOR_GRAY2BGR);
      else
        cimg = rimg;

      // cv::imshow("out", cimg);
      // cv::waitKey();

      cv::Mat canvasPart;
      if(!is_vertical)
        canvasPart = canvas(cv::Rect(show_size.width*k, 0,
                                      show_size.width,
                                      show_size.height));
      else
        canvasPart = canvas(cv::Rect(0, show_size.height*k,
                                      show_size.width,
                                      show_size.height));

      cv::resize(cimg, canvasPart, canvasPart.size(), 0, 0, CV_INTER_AREA);

      cv::Rect vroi(cvRound(valid_rois[k].x*size_factor),
                    cvRound(valid_rois[k].y*size_factor),
                    cvRound(valid_rois[k].width*size_factor),
                    cvRound(valid_rois[k].height*size_factor));
      cv::rectangle(canvasPart, vroi, cv::Scalar(0,0,255), 3, 8);

  }

  if( !is_vertical )
      for( int j = 0; j < canvas.rows; j += 16 )
          cv::line(canvas, cv::Point(0, j),
                    cv::Point(canvas.cols, j),
                    cv::Scalar(0, 255, 0), 1, 8);
  else
      for( int j = 0; j < canvas.cols; j += 16 )
          cv::line(canvas, cv::Point(j, 0),
                    cv::Point(j, canvas.rows),
                    cv::Scalar(0, 255, 0), 1, 8);
  cv::imshow("rectified", canvas);
  cv::waitKey();

}






int main(int argc, char const *argv[]) {

  cv::CommandLineParser parser(argc, argv, keys.c_str());

  if(argc < 3){
    help(parser);
    return 0;
  }

  InputParameters input_parameters;
  fillInputParans(parser, input_parameters);
  printParams(input_parameters);

  // Load process
  std::vector<cv::VideoCapture> captures;
  captures = initVideoCapture(input_parameters);

  // Chessboard detection process
  cv::Mat image_left, image_right, image_detect_chessboard;
  cv::Size image_size(0, 0);
  std::vector< std::vector<cv::Point2f> > vector_corners[2];

  int image_pair_count = 0;
  bool two_captures = captures.size() == 2;
  if (two_captures){
    while(captures[0].grab() && captures[1].grab()) {
      std::cout << "\tPair ID = " << image_pair_count++ <<" | ";
      captures[0].retrieve(image_left);
      captures[1].retrieve(image_right);

      if((image_size.height == 0) && (image_size.width == 0))
        image_size = image_left.size();

      std::vector<cv::Point2f> corners_left, corners_right;
      bool found_chessboard = findChessboard(image_left, image_right,
                                             input_parameters.board_size,
                                             image_detect_chessboard,
                                             corners_left, corners_right);

      if(found_chessboard){
        vector_corners[0].push_back(corners_left);
        vector_corners[1].push_back(corners_right);
      }
    // cv::imshow("Chessboard detect", image_detect_chessboard);
    // cv::waitKey();
    }

    std::vector<cv::Rect> valid_rois;
    std::vector< cv::Mat > mats;

    if( !calibrationProcess( input_parameters, vector_corners, image_size,
                            valid_rois, mats) ){
      return 0;
    }

    bool is_vertical = fabs(mats[11].at<double>(1, 3))
                       > fabs(mats[11].at<double>(0, 3));

    std::vector< std::vector <cv::Mat> > remap(2, std::vector<cv::Mat>(2));
    cv::initUndistortRectifyMap(mats[0], mats[2], mats[8], mats[10], image_size,
                               CV_16SC2, remap[0][0], remap[0][1]);
    cv::initUndistortRectifyMap(mats[1], mats[3], mats[9], mats[11], image_size,
                               CV_16SC2, remap[1][0], remap[1][1]);

    cv::Mat canvas;
    double size_factor;
    int width_temp, height_temp;
    if( !is_vertical ){
      size_factor = 600./MAX(image_size.width, image_size.height);
      width_temp = cvRound(image_size.width*size_factor);
      height_temp = cvRound(image_size.height*size_factor);
      canvas.create(height_temp, width_temp*2, CV_8UC3);
    }else{
      size_factor = 300./MAX(image_size.width, image_size.height);
      width_temp = cvRound(image_size.width*size_factor);
      height_temp = cvRound(image_size.height*size_factor);
      canvas.create(height_temp*2, width_temp, CV_8UC3);
    }

    cv::Size show_size(width_temp, height_temp);
    captures = initVideoCapture(input_parameters);
    while(captures[0].grab() && captures[1].grab()) {
      captures[0].retrieve(image_left);
      captures[1].retrieve(image_right);

      showRectified(image_left, image_right, remap, canvas, is_vertical,
                    show_size, size_factor, valid_rois);

    }




  }else{
    // TODO
  }









  return 0;
}
