
#include "Course.h"

Course::Course(string id, string name) {
    courseId = id;
    courseName = name;
}

string Course::getCourseId() const {
    return courseId;
}

string Course::getCourseName() const {
    return courseName;
}
