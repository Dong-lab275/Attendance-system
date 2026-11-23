
#include <iostream>
#include <string>
#include <limits>

#include "../ModuleLecturer/Controller/LecturerController.h"
#include "../ModuleLecturer/View/LecturerView.h"

using namespace std;

int main()
{
    LecturerController controller;
    LecturerView view;

    string selectedCourseId;

    cout << "============================================\n";
    cout << "       HE THONG QUAN LY DIEM DANH\n";
    cout << "              GIANG VIEN\n";
    cout << "============================================\n\n";

    while (true)
    {
        view.showMainMenu(
            controller.getLecturerSummary(),
            selectedCourseId
        );

        int choice = view.promptMenuChoice();
        string message;

        switch (choice)
        {
        case 1:
        {
            view.showAssignedCourses(controller.getAssignedCourses());
            break;
        }

        case 2:
        {
            view.showAssignedCourses(controller.getAssignedCourses());

            selectedCourseId = view.promptLine("Nhap ma hoc phan: ");

            bool found = false;

            for (const auto& course : controller.getAssignedCourses())
            {
                if (course.courseId == selectedCourseId)
                {
                    found = true;
                    break;
                }
            }

            if (found)
                view.showMessage("Da chon hoc phan thanh cong.");
            else
            {
                selectedCourseId.clear();
                view.showMessage(
                    "Khong tim thay hoc phan hoac hoc phan chua duoc phan cong."
                );
            }

            break;
        }

        case 3:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            view.showSessions(
                controller.getSessionsByCourse(selectedCourseId)
            );

            break;
        }

        case 4:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            int sessionNumber = view.promptInt("Nhap so buoi hoc: ");
            string date = view.promptLine("Nhap ngay hoc (YYYY-MM-DD): ");
            string startTime = view.promptLine("Nhap gio bat dau (HH:MM): ");
            string endTime = view.promptLine("Nhap gio ket thuc (HH:MM): ");
            string openTime = view.promptLine("Nhap gio mo diem danh (HH:MM): ");
            string closeTime = view.promptLine("Nhap gio ket thuc diem danh (HH:MM): ");

            controller.createClassSession(
                selectedCourseId,
                sessionNumber,
                date,
                startTime,
                endTime,
                openTime,
                closeTime,
                message
            );

            view.showMessage(message);
            break;
        }

        case 5:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            int sessionNumber = view.promptInt("Nhap so buoi hoc: ");
            string openTime = view.promptLine("Nhap gio mo diem danh moi: ");
            string endTime = view.promptLine("Nhap gio ket thuc diem danh moi: ");

            cout << "\nCac trang thai hop le:\n";
            cout << "1. Scheduled - Da len lich\n";
            cout << "2. Open - Dang mo diem danh\n";
            cout << "3. Closed - Da dong diem danh\n";
            cout << "4. Completed - Da hoan thanh\n";

            string status = view.promptLine("Nhap trang thai moi: ");

            controller.updateSessionWindow(
                selectedCourseId,
                sessionNumber,
                openTime,
                endTime,
                status,
                message
            );

            view.showMessage(message);
            break;
        }

        case 6:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            int sessionNumber = view.promptInt("Nhap so buoi hoc: ");

            auto records = controller.getSessionAttendance(
                selectedCourseId,
                sessionNumber
            );

            view.showSessionAttendance(
                records,
                controller.calculateSessionStatistics(
                    selectedCourseId,
                    sessionNumber
                )
            );

            cout << "\n========== QUAN LY DIEM DANH ==========\n";
            cout << "1. Ghi nhan gio diem danh\n";
            cout << "2. Cap nhat trang thai diem danh thu cong\n";
            cout << "3. Dong phien diem danh va danh dau vang\n";
            cout << "0. Quay lai\n";
            cout << "=======================================\n";

            int action = view.promptMenuChoice();

            if (action == 1)
            {
                string studentId = view.promptLine("Nhap ma sinh vien: ");
                string checkInTime = view.promptLine("Nhap gio diem danh (HH:MM): ");

                controller.recordStudentCheckInOrManual(
                    selectedCourseId,
                    sessionNumber,
                    studentId,
                    checkInTime,
                    false,
                    message
                );

                view.showMessage(message);
            }
            else if (action == 2)
            {
                string studentId = view.promptLine("Nhap ma sinh vien: ");

                cout << "\nCac trang thai hop le:\n";
                cout << "Present - Co mat\n";
                cout << "Late - Di tre\n";
                cout << "Absent - Vang mat\n";

                string status = view.promptLine("Nhap trang thai moi: ");

                controller.recordStudentCheckInOrManual(
                    selectedCourseId,
                    sessionNumber,
                    studentId,
                    status,
                    true,
                    message
                );

                view.showMessage(message);
            }
            else if (action == 3)
            {
                controller.closeSessionAndMarkAbsent(
                    selectedCourseId,
                    sessionNumber,
                    message
                );

                view.showMessage(message);
            }
            else if (action != 0)
            {
                view.showMessage("Lua chon khong hop le.");
            }

            break;
        }

        case 7:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            string keywordName = view.promptLine(
                "Nhap ten sinh vien can tim (co the bo trong): "
            );

            string keywordId = view.promptLine(
                "Nhap ma sinh vien can tim (co the bo trong): "
            );

            view.showStudents(
                controller.searchAndFilterStudents(
                    selectedCourseId,
                    keywordName,
                    keywordId
                )
            );

            break;
        }

        case 8:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            string studentId = view.promptLine("Nhap ma sinh vien: ");

            cout << "\nCac trang thai loc:\n";
            cout << "All - Tat ca\n";
            cout << "Present - Co mat\n";
            cout << "Late - Di tre\n";
            cout << "Absent - Vang mat\n";

            string statusFilter = view.promptLine(
                "Nhap trang thai can loc: "
            );

            if (statusFilter.empty())
                statusFilter = "All";

            auto history = controller.getStudentHistoryInCourse(
                selectedCourseId,
                studentId,
                statusFilter
            );

            auto statistics = controller.calculateStudentStatistics(
                selectedCourseId,
                studentId
            );

            view.showStudentHistoryAndStats(
                studentId,
                history,
                statistics
            );

            break;
        }

        case 9:
        {
            if (selectedCourseId.empty())
            {
                view.showMessage("Vui long chon hoc phan truoc.");
                break;
            }

            int sessionNumber = view.promptInt(
                "Nhap so buoi hoc can xuat bao cao: "
            );

            view.showReportExport(
                controller.exportSessionReportText(
                    selectedCourseId,
                    sessionNumber
                )
            );

            break;
        }

        case 10:
        {
            view.showPersonalInfo(
                controller.getPersonalInfoText()
            );

            break;
        }

        case 0:
        {
            cout << "\nDang thoat khoi he thong giang vien...\n";
            cout << "Cam on ban da su dung he thong!\n";
            return 0;
        }

        default:
        {
            view.showMessage(
                "Lua chon khong hop le. Vui long chon lai."
            );
            break;
        }
        }
    }

    return 0;
}
