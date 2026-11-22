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
   0. ĐỒNG BỘ LINK SIDEBAR THEO ĐÚNG TÊN FILE HIỆN TẠI
   ========================================================= */
function initSidebarNavigation() {
  const routeMap = {
    'class session management': 'Class-Sessions.html',
    'manual attendance': 'attendance-register.html',
    'student search & filtering': 'studentsearch-filtering.html',
    'attendance statistics': 'Student-statistics.html',
    'attendance history': 'attendance-history.html',
    'report export': 'report-export.html',
    'personal information': 'personal-information.html'
  };

  const navLinks = document.querySelectorAll('.role-nav .nav-item, .sidebar a');
  const currentFile = decodeURIComponent(window.location.pathname.split('/').pop() || '').toLowerCase();

  navLinks.forEach((link) => {
    const labelText = (link.textContent || '')
      .replace(/\s+/g, ' ')
      .trim()
      .toLowerCase();

    const targetFile = routeMap[labelText];
    if (!targetFile) return;

    link.setAttribute('href', `./${targetFile}`);
    link.classList.toggle('active', currentFile === targetFile.toLowerCase());

    link.addEventListener('click', (e) => {
      if (e.metaKey || e.ctrlKey || e.shiftKey || e.altKey || e.button !== 0) return;
      e.preventDefault();

      const currentUrl = new URL(window.location.href);
      const baseDir = currentUrl.href.slice(0, currentUrl.href.lastIndexOf('/') + 1);
      window.location.href = `${baseDir}${targetFile}`;
    });
  });
}

/* =========================================================
   1. ĐIỂM DANH THỦ CÔNG (attendance-register.html)
   ========================================================= */
function initAttendanceRegister() {
  const rows = document.querySelectorAll('.student-row');
  if (!rows.length) return;

  const searchInput = document.getElementById('roster-search');
  const statusSelect = document.getElementById('status-filter');
  const shownCount = document.getElementById('shown-count');
  const savedBadge = document.querySelector('.badge-saved');

  function getRowStatus(row) {
    if (row.querySelector('.choice-btn.is-present')) return 'present';
    if (row.querySelector('.choice-btn.is-late')) return 'late';
    if (row.querySelector('.choice-btn.is-absent')) return 'absent';
    return 'none';
  }

  function recalculateSummary() {
    let present = 0;
    let late = 0;
    let absent = 0;

    rows.forEach((row) => {
      const st = getRowStatus(row);
      if (st === 'present') present++;
      if (st === 'late') late++;
      if (st === 'absent') absent++;
    });

    const statValues = document.querySelectorAll('.summary-grid .stat-value');
    if (statValues.length >= 4) {
      statValues[1].textContent = present;
      statValues[2].textContent = late;
      statValues[3].textContent = absent;
    }
  }

  function applyRosterFilters() {
    const query = (searchInput ? searchInput.value : '').trim().toLowerCase();
    const statusVal = (statusSelect ? statusSelect.value : 'all').toLowerCase();
    let visible = 0;

    rows.forEach((row) => {
      const text = (row.textContent || '').toLowerCase();
      const st = getRowStatus(row);
      const matchQuery = !query || text.includes(query);
      const matchStatus = statusVal === 'all' || st === statusVal;
      const show = matchQuery && matchStatus;

      row.style.display = show ? 'flex' : 'none';
      if (show) visible++;
    });

    if (shownCount) {
      shownCount.textContent = `Showing ${visible} of 40 students`;
    }
  }

  rows.forEach((row) => {
    const buttons = row.querySelectorAll('.choice-btn');
    buttons.forEach((btn) => {
      btn.addEventListener('click', () => {
        buttons.forEach((b) => b.classList.remove('is-present', 'is-late', 'is-absent'));

        const status = btn.textContent.trim();
        if (status === 'Present') btn.classList.add('is-present');
        if (status === 'Late') btn.classList.add('is-late');
        if (status === 'Absent') btn.classList.add('is-absent');

        if (savedBadge) {
          savedBadge.textContent = 'Unsaved changes';
          savedBadge.classList.add('is-dirty');
        }

        recalculateSummary();
        applyRosterFilters();
      });
    });
  });

  if (searchInput) searchInput.addEventListener('input', applyRosterFilters);
  if (statusSelect) statusSelect.addEventListener('change', applyRosterFilters);

  const saveBtn = document.getElementById('btn-save') || document.querySelector('.page-heading .btn-primary');
  if (saveBtn) {
    saveBtn.addEventListener('click', () => {
      if (savedBadge) {
        savedBadge.textContent = 'Saved just now';
        savedBadge.classList.remove('is-dirty');
      }
      alert('Đã lưu thay đổi điểm danh trên trình duyệt!');
    });
  }
}

/* =========================================================
   2. QUẢN LÝ BUỔI HỌC (Class-Sessions.html)
   ========================================================= */
function initClassSessions() {
  const sessionsCard = document.querySelector('.sessions-card');
  const submitBtn = document.getElementById('btn-submit') || document.querySelector('.editor-actions .btn-primary');
  const cancelBtn = document.getElementById('btn-cancel') || document.querySelector('.editor-actions .btn-secondary');
  const dateInput = document.getElementById('f-date');
  const startInput = document.getElementById('f-start');
  const endInput = document.getElementById('f-end');
  const openInput = document.getElementById('f-open');
  const periodInput = document.getElementById('f-period');
  const autoCloseSpan = document.getElementById('auto-close');
  const errorEl = document.getElementById('form-error');

  if (!sessionsCard || !submitBtn) return;

  let nextSessionNumber = 16;

  function updateAutoClose() {
    if (!periodInput || !autoCloseSpan) return;
    const match = periodInput.value.trim().match(/^(\d{1,2}):(\d{2})$/);
    if (!match) return;
    const totalMins = parseInt(match[1], 10) * 60 + parseInt(match[2], 10) + 15;
    const hh = String(Math.floor((totalMins / 60) % 24)).padStart(2, '0');
    const mm = String(totalMins % 60).padStart(2, '0');
    autoCloseSpan.textContent = `${hh}:${mm}`;
  }

  if (periodInput) {
    periodInput.addEventListener('input', updateAutoClose);
    updateAutoClose();
  }

  submitBtn.addEventListener('click', () => {
    const sessionDate = dateInput ? dateInput.value.trim() : '02 Dec 2026';
    const startTime = startInput ? startInput.value.trim() : '09:00';

    if (!sessionDate || !startTime) {
      if (errorEl) {
        errorEl.textContent = 'Vui lòng nhập đầy đủ ngày học và giờ bắt đầu.';
        errorEl.hidden = false;
      }
      return;
    }

    if (errorEl) {
      errorEl.textContent = '';
      errorEl.hidden = true;
    }

    const newRow = document.createElement('div');
    newRow.className = 'table-row';
    newRow.dataset.date = sessionDate;
    newRow.dataset.start = startTime;
    newRow.dataset.end = endInput ? endInput.value.trim() : '11:00';
    newRow.dataset.open = openInput ? openInput.value.trim() : '09:00';
    newRow.dataset.period = periodInput ? periodInput.value.trim() : '09:15';
    newRow.innerHTML = `
      <div class="col-session">${nextSessionNumber}</div>
      <div class="col-schedule">${sessionDate.replace(' 2026', '')} · ${startTime}</div>
      <div class="col-attendance"><span class="badge badge-scheduled">Scheduled</span></div>
      <div class="col-action"><span class="row-actions"><a href="#" class="action-link" data-action="view">View</a> / <a href="#" class="action-link" data-action="edit">Edit</a> / <a href="#" class="action-link" data-action="delete">Delete</a></span></div>
    `;

    sessionsCard.appendChild(newRow);
    nextSessionNumber++;
  });

  sessionsCard.addEventListener('click', (e) => {
    const link = e.target.closest('.action-link');
    if (!link) return;
    e.preventDefault();

    const row = link.closest('.table-row');
    const action = link.dataset.action;

    if (action === 'delete' && row) {
      row.remove();
    } else if (action === 'edit' && row) {
      if (dateInput && row.dataset.date) dateInput.value = row.dataset.date;
      if (startInput && row.dataset.start) startInput.value = row.dataset.start;
      if (endInput && row.dataset.end) endInput.value = row.dataset.end;
      if (openInput && row.dataset.open) openInput.value = row.dataset.open;
      if (periodInput && row.dataset.period) periodInput.value = row.dataset.period;
      updateAutoClose();
    } else if (action === 'view' && row) {
      const sessionNo = row.querySelector('.col-session')?.textContent.trim() || '';
      const schedule = row.querySelector('.col-schedule')?.textContent.trim() || '';
      alert(`Buổi học #${sessionNo} — ${schedule}`);
    }
  });

  if (cancelBtn) {
    cancelBtn.addEventListener('click', () => {
      if (errorEl) {
        errorEl.textContent = '';
        errorEl.hidden = true;
      }
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
  const studentSelect = document.getElementById('student-filter');
  const rateSelect = document.getElementById('rate-filter');
  const emptyBox = document.getElementById('stats-empty');

  if (!statsRows.length || summaryValues.length < 3) return;

  function selectRow(row) {
    statsRows.forEach((r) => r.classList.remove('is-selected'));
    row.classList.add('is-selected');

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
  }

  function applyStatsFilters() {
    const selectedId = studentSelect ? studentSelect.value : 'all';
    const rateBand = rateSelect ? rateSelect.value : 'all';
    let visibleCount = 0;

    statsRows.forEach((row) => {
      const text = row.textContent || '';
      const rateText = row.children[3]?.textContent || '0';
      const rateNum = parseFloat(rateText.replace('%', '')) || 0;

      const matchStudent = selectedId === 'all' || text.includes(selectedId);
      let matchRate = true;
      if (rateBand === 'high') matchRate = rateNum >= 90;
      if (rateBand === 'mid') matchRate = rateNum >= 75 && rateNum < 90;
      if (rateBand === 'low') matchRate = rateNum < 75;

      const show = matchStudent && matchRate;
      row.style.display = show ? 'grid' : 'none';
      if (show) visibleCount++;
    });

    if (emptyBox) {
      emptyBox.hidden = visibleCount > 0;
    }
  }

  statsRows.forEach((row) => {
    row.addEventListener('click', () => selectRow(row));
  });

  if (studentSelect) studentSelect.addEventListener('change', applyStatsFilters);
  if (rateSelect) rateSelect.addEventListener('change', applyStatsFilters);
}

/* =========================================================
   4. TÌM KIẾM & LỌC SINH VIÊN (studentsearch-filtering.html)
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
    nameInput.addEventListener('input', runFilter);
    nameInput.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') runFilter();
    });
  }

  if (idInput) {
    idInput.addEventListener('input', runFilter);
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