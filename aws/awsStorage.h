#ifndef REAL_FRAMEWORK_AWSSTORAGE_H_
#define REAL_FRAMEWORK_AWSSTORAGE_H_

#include "aws_s3.h"
#include "storage.h"
#include "opencv2/opencv.hpp"

class awsStorage : public rea::fsStorage {
public:
    awsStorage(const QString& aType = "", const QJsonObject& aConfig = QJsonObject());
    bool isValid() override { return m_aws.isValid(); }
    void initialize() override;
protected:
    std::vector<QString> listFiles(const QString& aDirectory) override;
    bool writeJsonObject(const QString& aPath, const QJsonObject& aData) override;
    bool writeByteArray(const QString& aPath, const QByteArray& aData) override;
    bool writeImage(const QString&, const QImage&) override{
        return false;
    }
    bool readJsonObject(const QString& aPath, QJsonObject& aData) override;
    bool readByteArray(const QString& aPath, QByteArray& aData) override;
    bool readImage(const QString&, QImage&) override{
        return false;
    }
    std::vector<QString> getFileList(const QString& aPath) override;
    void deletePath(const QString& aPath) override;
#ifdef USEOPENCV
    bool writeCVMat(const QString& aPath, const cv::Mat& aData);
    bool readCVMat(const QString& aPath, cv::Mat& aData);
#endif
protected:
    AWSClient m_aws;
};

#endif
