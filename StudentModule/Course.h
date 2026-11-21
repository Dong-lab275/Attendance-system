
#ifndef COURSE_H
#define COURSE_H

#include <string>
using namespace std;

class Course {
private:
    string courseId;
    string courseName;

public:
    Course(string id, string name);

    string getCourseId() const;
    string getCourseName() const;
};

#endif
