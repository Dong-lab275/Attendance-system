document.addEventListener('DOMContentLoaded', () => {
  initSidebarNavigation();
  initAttendanceRegister();
  initClassSessions();
  initStudentStatisticsFilter();
  initStudentSearchFilter();
  initAttendanceHistoryFilter();
  initReportExport();
  initAccountActions();
});

/* =========================================================
   0. TỰ ĐỘNG ĐỒNG BỘ LINK SIDEBAR CHO MỌI TRANG
   ========================================================= */
function initSidebarNavigation() {
  const routeMap = {
    'class session management': 'Class-Sessions.html',
    'manual attendance': 'attendance.html',
    'student search & filtering': 'student-search.html',
    'attendance statistics': 'Student-statistics.html',
    'attendance history': 'attendance-history.html',
    'report export': 'report-export.html',
    'personal information': 'personal-information.html'
  };

  const navLinks = document.querySelectorAll('.role-nav .nav-item, .sidebar a');
  const currentFile = (window.location.pathname.split('/').pop() || '').toLowerCase();

  navLinks.forEach((link) => {
    const labelText = (link.textContent || '')
      .replace(/\s+/g, ' ')
      .trim()
      .toLowerCase();

    const targetFile = routeMap[labelText];
    if (targetFile) {
      link.setAttribute('href', `./${targetFile}`);
      if (currentFile === targetFile.toLowerCase()) {
        link.classList.add('active');
      }
    }
  });
}

/* =========================================================
   1. ĐIỂM DANH THỦ CÔNG (attendance.html)
   ========================================================= */
function initAttendanceRegister() {
  const rows = document.querySelectorAll('.student-row');
  if (!rows.length) return;

  function recalculateSummary() {
    let present = 0;
    let late = 0;
    let absent = 0;

    rows.forEach((row) => {
      if (row.querySelector('.choice-btn.is-present')) present++;
      if (row.querySelector('.choice-btn.is-late')) late++;
      if (row.querySelector('.choice-btn.is-absent')) absent++;
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
   2. QUẢN LÝ BUỔI HỌC (Class-Sessions.html)
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
   3. THỐNG KÊ SINH VIÊN (Student-statistics.html)
   ========================================================= */
function initStudentStatisticsFilter() {
  const statsRows = document.querySelectorAll('.stats-table-row');
  const summaryValues = document.querySelectorAll('.stats-summary-grid .stat-value');
  const summarySubs = document.querySelectorAll('.stats-summary-grid .stat-sub');

  if (!statsRows.length || summaryValues.length < 3) return;

  statsRows.forEach((row) => {
    row.style.cursor = 'pointer';
    row.addEventListener('click', () => {
      const cols = row.children;
      if (cols.length < 5) return;

      const fullLabel = cols[0].textContent.trim();
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
   4. TÌM KIẾM & LỌC SINH VIÊN (student-search.html)
   ========================================================= */
function initStudentSearchFilter() {
  const applyBtn = document.getElementById('applyStudentFilters');
  const resetBtn = document.getElementById('resetStudentFilters');
  const nameInput = document.getElementById('filterStudentName');
  const idInput = document.getElementById('filterStudentId');
  const courseInput = document.getElementById('filterCourse');
  const rows = document.querySelectorAll('.students-table-row');
  const showingText = document.getElementById('studentShowingText');

  if (!applyBtn || !rows.length) return;

  function runFilter() {
    const nameQuery = (nameInput ? nameInput.value : '').trim().toLowerCase();
    const idQuery = (idInput ? idInput.value : '').trim().toLowerCase();
    const courseQuery = (courseInput ? courseInput.value : '').trim().toLowerCase();

    let visibleCount = 0;

    rows.forEach((row) => {
      const studentName = (row.querySelector('.student-col-name')?.textContent || '').trim().toLowerCase();
      const studentId = (row.querySelector('.student-col-id')?.textContent || '').trim().toLowerCase();
      const rowCourse = (row.dataset.course || '').trim().toLowerCase();

      const matchName = !nameQuery || studentName.includes(nameQuery);
      const matchId = !idQuery || studentId.includes(idQuery);
      const matchCourse = !courseQuery || rowCourse.includes(courseQuery) || courseQuery.includes(rowCourse);

      const isMatch = matchName && matchId && matchCourse;
      row.style.display = isMatch ? 'grid' : 'none';
      if (isMatch) visibleCount++;
    });

    if (showingText) {
      showingText.textContent = `Showing ${visibleCount} of 40 students`;
    }
  }

  applyBtn.addEventListener('click', runFilter);

  if (nameInput) {
    nameInput.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') runFilter();
    });
  }

  if (idInput) {
    idInput.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') runFilter();
    });
  }

  if (resetBtn) {
    resetBtn.addEventListener('click', () => {
      if (nameInput) nameInput.value = '';
      if (idInput) idInput.value = '';
      if (courseInput) courseInput.value = 'Cơ sở dữ liệu · CS201';
      runFilter();
    });
  }
}

/* =========================================================
   5. LỊCH SỬ ĐIỂM DANH (attendance-history.html)
   ========================================================= */
function initAttendanceHistoryFilter() {
  const statusChip = document.getElementById('historyStatusChip');
  const studentChip = document.getElementById('historyStudentChip');
  const studentTitle = document.getElementById('historyStudentTitle');
  const studentMeta = document.getElementById('historyStudentMeta');
  const rows = document.querySelectorAll('.history-table-row');
  const showingText = document.getElementById('historyShowingText');

  if (!rows.length) return;

  const statusModes = ['All attendance statuses', 'Present', 'Late', 'Absent'];
  let statusIndex = 0;

  function applyHistoryFilter() {
    const selectedStatus = statusModes[statusIndex];
    let visibleCount = 0;

    rows.forEach((row) => {
      const rowStatus = row.dataset.status || '';
      const isMatch = selectedStatus === 'All attendance statuses' || rowStatus === selectedStatus;
      row.style.display = isMatch ? 'grid' : 'none';
      if (isMatch) visibleCount++;
    });

    if (showingText) {
      showingText.textContent = `Showing ${visibleCount} of 12 sessions`;
    }
  }

  if (statusChip) {
    statusChip.addEventListener('click', () => {
      statusIndex = (statusIndex + 1) % statusModes.length;
      statusChip.textContent = statusModes[statusIndex];
      applyHistoryFilter();
    });
  }

  const sampleStudents = [
    {
      label: 'Nguyễn Minh Anh · SV2024001',
      name: 'Nguyễn Minh Anh',
      meta: 'SV2024001 · Cơ sở dữ liệu · 12 completed sess'
    },
    {
      label: 'Trần Đức Anh · SV2024002',
      name: 'Trần Đức Anh',
      meta: 'SV2024002 · Cơ sở dữ liệu · 12 completed sess'
    },
    {
      label: 'Lê Ngọc Mai · SV2024003',
      name: 'Lê Ngọc Mai',
      meta: 'SV2024003 · Cơ sở dữ liệu · 12 completed sess'
    }
  ];
  let studentIndex = 0;

  if (studentChip) {
    studentChip.addEventListener('click', () => {
      studentIndex = (studentIndex + 1) % sampleStudents.length;
      const currentStudent = sampleStudents[studentIndex];
      studentChip.textContent = currentStudent.label;
      if (studentTitle) studentTitle.textContent = currentStudent.name;
      if (studentMeta) studentMeta.textContent = currentStudent.meta;
    });
  }
}

/* =========================================================
   6. XUẤT BÁO CÁO ĐIỂM DANH (report-export.html)
   ========================================================= */
function initReportExport() {
  const scopeInput = document.getElementById('reportScope');
  const formatInput = document.getElementById('reportFormat');
  const metaLine = document.getElementById('previewMetaLine');
  const exportButtons = document.querySelectorAll('.export-pdf-btn');

  if (!exportButtons.length) return;

  if (scopeInput && metaLine) {
    scopeInput.addEventListener('input', () => {
      const scopeVal = scopeInput.value.trim() || 'Session 12 · 04 Nov 2026';
      metaLine.innerHTML = `CS201 · ${scopeVal}<br />Lecturer: Trần Quốc Huy (GV001)`;
    });
  }

  exportButtons.forEach((btn) => {
    btn.addEventListener('click', () => {
      const fmt = (formatInput ? formatInput.value.trim() : 'PDF') || 'PDF';
      const scope = (scopeInput ? scopeInput.value.trim() : 'Session 12 · 04 Nov 2026');
      alert(`Đang chuẩn bị xuất báo cáo (${fmt}) cho phạm vi: ${scope}.`);
    });
  });
}

/* =========================================================
   7. CÁC NÚT TÀI KHOẢN CHUNG
   ========================================================= */
function initAccountActions() {
  const changePwdBtns = document.querySelectorAll('.security-card .btn-secondary, .account-actions .account-link');
  changePwdBtns.forEach((el) => {
    const text = el.textContent.trim().toLowerCase();
    if (text.includes('change password')) {
      el.addEventListener('click', (e) => {
        e.preventDefault();
        alert('Tính năng đổi mật khẩu sẽ được kết nối với hệ thống xác thực.');
      });
    }
    if (text.includes('sign out')) {
      el.addEventListener('click', (e) => {
        e.preventDefault();
        alert('Bạn đã đăng xuất khỏi phiên làm việc hiện tại.');
      });
    }
  });
}