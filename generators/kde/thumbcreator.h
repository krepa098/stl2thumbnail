#pragma once

#include <KIO/ThumbCreator>

class StlThumbCreator : public ThumbCreator
{
public:
    StlThumbCreator();

    virtual bool create(const QString &path, int width, int height, QImage &img);
};

