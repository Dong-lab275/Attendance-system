#include <iostream>
#include <string>
#include <vector>
#include "Controller/LecturerController.h"
#include "View/LecturerView.h"

using namespace std;

int main() {
    LecturerController controller;
    LecturerView view;
    string currentCourseId = "CS201";

    while (true) {
        view.showMainMenu(controller.getLecturerSummary(), currentCourseId);
        int choice = view.promptMenuChoice();

        if (choice == 0) {
            view.showMessage("Thoat chuong trinh module giang vien.");
            break;
        }
        if (choice == 1) {
            view.showAssignedCourses(controller.getAssignedCourses());
        } else if (choice == 2) {
            view.showAssignedCourses(controller.getAssignedCourses());
            string cid = view.promptLine("Nhap ma khoa hoc (VD: CS201): ");
            currentCourseId = cid;
            view.showMessage("Da chuyen sang khoa hoc " + currentCourseId);
        } else if (choice == 3) {
            view.showSessions(controller.getSessionsByCourse(currentCourseId));
        } else if (choice == 4) {
            int sNum = view.promptInt("Nhap so buoi hoc (VD: 16): ");
            string dateStr = view.promptLine("Nhap ngay hoc (VD: 02 Dec 2026): ");
            string startT = view.promptLine("Gio bat dau lop (VD: 09:00): ");
            string endT = view.promptLine("Gio ket thuc lop (VD: 11:00): ");
            string openT = view.promptLine("Gio mo diem danh (VD: 09:00): ");
            string endCheckT = view.promptLine("Gio ket thuc khung diem danh (VD: 09:15): ");
            string msg;
            controller.createClassSession(currentCourseId, sNum, dateStr, startT, endT, openT, endCheckT, msg);
            view.showMessage(msg);
        } else if (choice == 5) {
            view.showSessions(controller.getSessionsByCourse(currentCourseId));
            int sNum = view.promptInt("Nhap so buoi hoc can cap nhat/dong: ");
            string action = view.promptLine("Nhap 'close' de dong phien & tu danh Absent, hoac 'edit' de sua gio: ");
            string msg;
            if (action == "close") {
                controller.closeSessionAndMarkAbsent(currentCourseId, sNum, msg);
            } else {
                string openT = view.promptLine("Gio mo diem danh moi (VD: 09:00): ");
                string endT = view.promptLine("Gio ket thuc khung moi (VD: 09:15): ");
                string st = view.promptLine("Trang thai (Scheduled / Open / Closed): ");
                controller.updateSessionWindow(currentCourseId, sNum, openT, endT, st, msg);
            }
            view.showMessage(msg);
        } else if (choice == 6) {
            int sNum = view.promptInt("Nhap so buoi hoc (VD: 12 hoac 13): ");
            string sid = view.promptLine("Nhap ma sinh vien (VD: SV2024004): ");
            string mode = view.promptLine("Chon che do ('time' de tu dong xet Present/Late theo gio, 'manual' de chon trang thai): ");
            string msg;
            if (mode == "time") {
                string t = view.promptLine("Nhap gio check-in (VD: 09:05 hoac 09:20): ");
                controller.recordStudentCheckInOrManual(currentCourseId, sNum, sid, t, false, msg);
            } else {
                string st = view.promptLine("Nhap trang thai (Present / Late / Absent): ");
                controller.recordStudentCheckInOrManual(currentCourseId, sNum, sid, st, true, msg);
            }
            view.showMessage(msg);
            view.showSessionAttendance(controller.getSessionAttendance(currentCourseId, sNum),
                                       controller.calculateSessionStatistics(currentCourseId, sNum));
        } else if (choice == 7) {
            string kwName = view.promptLine("Loc theo ten sinh vien (bo trong neu bo qua): ");
            string kwId = view.promptLine("Loc theo ma sinh vien (bo trong neu bo qua): ");
            view.showStudents(controller.searchAndFilterStudents(currentCourseId, kwName, kwId));
        } else if (choice == 8) {
            string sid = view.promptLine("Nhap ma sinh vien (VD: SV2024001): ");
            string filter = view.promptLine("Loc trang thai (All / Present / Late / Absent): ");
            view.showStudentHistoryAndStats(sid,
                                            controller.getStudentHistoryInCourse(currentCourseId, sid, filter),
                                            controller.calculateStudentStatistics(currentCourseId, sid));
        } else if (choice == 9) {
            int sNum = view.promptInt("Nhap so buoi hoc can xuat bao cao (VD: 12): ");
            view.showReportExport(controller.exportSessionReportText(currentCourseId, sNum));
        } else if (choice == 10) {
            view.showPersonalInfo(controller.getPersonalInfoText());
        } else {
            view.showMessage("Lua chon khong hop le.");
        }
    }

    return 0;
}