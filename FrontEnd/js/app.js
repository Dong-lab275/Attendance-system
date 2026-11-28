document.addEventListener('DOMContentLoaded', () => {
  initAttendanceRegister();
  initClassSessions();
  initStudentStatisticsFilter();
  initAccountActions();
});

/* =========================================================
   1. TRANG 09: ĐIỂM DANH THỦ CÔNG (attendance-register.html)
   ========================================================= */
function initAttendanceRegister() {
  const rows = document.querySelectorAll('.student-row');
  if (!rows.length) return;

  function recalculateSummary() {
    let present = 0;
    let late = 0;
    let absent = 0;

    rows.forEach((row) => {
      const hasPresent = row.querySelector('.choice-btn.is-present');
      const hasLate = row.querySelector('.choice-btn.is-late');
      const hasAbsent = row.querySelector('.choice-btn.is-absent');

      if (hasPresent) present++;
      if (hasLate) late++;
      if (hasAbsent) absent++;
    });

    const statValues = document.querySelectorAll('.summary-grid .stat-value');
    if (statValues.length >= 4) {
      statValues[1].textContent = present;
      statValues[2].textContent = late;
      statValues[3].textContent = absent;
    }
  }

  rows.forEach((row) => {
    const buttons = row.querySelectorAll('.choice-btn');
    buttons.forEach((btn) => {
      btn.addEventListener('click', () => {
        buttons.forEach((b) => {
          b.classList.remove('is-present', 'is-late', 'is-absent');
        });

        const status = btn.textContent.trim();
        if (status === 'Present') btn.classList.add('is-present');
        if (status === 'Late') btn.classList.add('is-late');
        if (status === 'Absent') btn.classList.add('is-absent');

        recalculateSummary();
      });
    });
  });

  // Lọc nhanh trạng thái khi bấm chip "All statuses"
  const filterChips = document.querySelectorAll('.roster-controls .filter-chip');
  const statusFilterChip = filterChips[1];
  const filterModes = ['All statuses', 'Present only', 'Late only', 'Absent only'];
  let modeIndex = 0;

  if (statusFilterChip) {
    statusFilterChip.addEventListener('click', () => {
      modeIndex = (modeIndex + 1) % filterModes.length;
      const currentMode = filterModes[modeIndex];
      statusFilterChip.textContent = currentMode;

      rows.forEach((row) => {
        const isPresent = Boolean(row.querySelector('.choice-btn.is-present'));
        const isLate = Boolean(row.querySelector('.choice-btn.is-late'));
        const isAbsent = Boolean(row.querySelector('.choice-btn.is-absent'));

        let show = true;
        if (currentMode === 'Present only') show = isPresent;
        if (currentMode === 'Late only') show = isLate;
        if (currentMode === 'Absent only') show = isAbsent;

        row.style.display = show ? 'flex' : 'none';
      });
    });
  }

  // Nút Save attendance
  const saveBtn = document.querySelector('.page-heading .btn-primary');
  if (saveBtn) {
    saveBtn.addEventListener('click', () => {
      const badge = document.querySelector('.badge-saved');
      if (badge) {
        badge.textContent = 'Saved just now';
      }
      alert('Đã lưu thay đổi điểm danh trên trình duyệt!');
    });
  }
}

/* =========================================================
   2. TRANG 08: QUẢN LÝ BUỔI HỌC (class-sessions.html)
   ========================================================= */
function initClassSessions() {
  const editorCreateBtn = document.querySelector('.editor-actions .btn-primary');
  const cancelBtn = document.querySelector('.editor-actions .btn-secondary');
  const sessionsCard = document.querySelector('.sessions-card');

  if (!editorCreateBtn || !sessionsCard) return;

  let nextSessionNumber = 16;

  editorCreateBtn.addEventListener('click', () => {
    const fieldBoxes = document.querySelectorAll('.session-editor .field-box');
    const sessionDate = fieldBoxes[1] ? fieldBoxes[1].textContent.trim() : '02 Dec 2026';
    const startTime = fieldBoxes[2] ? fieldBoxes[2].textContent.trim() : '09:00';

    const newRow = document.createElement('div');
    newRow.className = 'table-row';
    newRow.innerHTML = `
      <div class="col-session">${nextSessionNumber}</div>
      <div class="col-schedule">${sessionDate.replace(' 2026', '')} · ${startTime}</div>
      <div class="col-attendance"><span class="badge badge-scheduled">Scheduled</span></div>
      <div class="col-action"><a href="#" class="action-link">View / Edit / Delete</a></div>
    `;

    sessionsCard.appendChild(newRow);
    nextSessionNumber++;
  });

  if (cancelBtn) {
    cancelBtn.addEventListener('click', () => {
      alert('Đã hủy thao tác tạo buổi học.');
    });
  }
}

/* =========================================================
   3. TRANG 10: THỐNG KÊ SINH VIÊN (student-statistics.html)
   ========================================================= */
function initStudentStatisticsFilter() {
  const statsRows = document.querySelectorAll('.stats-table-row');
  const summaryValues = document.querySelectorAll('.stats-summary-grid .stat-value');
  const summarySubs = document.querySelectorAll('.stats-summary-grid .stat-sub');

  if (!statsRows.length || summaryValues.length < 3) return;

  // Bấm vào từng dòng sinh viên để cập nhật 3 thẻ thống kê phía trên
  statsRows.forEach((row) => {
    row.style.cursor = 'pointer';
    row.addEventListener('click', () => {
      const cols = row.children;
      if (cols.length < 5) return;

      const fullLabel = cols[0].textContent.trim(); // Ví dụ: "Trần Đức Anh · SV2024002"
      const parts = fullLabel.split('·');
      const studentName = parts[0] ? parts[0].trim() : fullLabel;
      const studentId = parts[1] ? parts[1].trim() : '';

      const completed = cols[1].textContent.trim();
      const attended = cols[2].textContent.trim();
      const attendanceRate = cols[3].textContent.trim();
      const onTimeRate = cols[4].textContent.trim();

      summaryValues[0].textContent = studentName;
      if (summarySubs[0]) summarySubs[0].textContent = studentId;

      summaryValues[1].textContent = attendanceRate;
      if (summarySubs[1]) summarySubs[1].textContent = `${attended} attended / ${completed} sessions`;

      summaryValues[2].textContent = onTimeRate;
      if (summarySubs[2]) summarySubs[2].textContent = `On-time rate: ${onTimeRate}`;
    });
  });
}

/* =========================================================
   4. CÁC NÚT TÀI KHOẢN CHUNG
   ========================================================= */
function initAccountActions() {
  const changePwdBtns = document.querySelectorAll('.security-card .btn-secondary, .account-actions .account-link');
  changePwdBtns.forEach((el) => {
    if (el.textContent.trim().toLowerCase().includes('change password')) {
      el.addEventListener('click', (e) => {
        e.preventDefault();
        alert('Tính năng đổi mật khẩu sẽ được kết nối với hệ thống xác thực.');
      });
    }
{      el.addEventListener('click', (e) => {
        e.preventDefault();
        alert('Bạn đã đăng xuất khỏi phiên làm việc hiện tại.');
      });
    }
  });
}