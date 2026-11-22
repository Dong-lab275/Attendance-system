
document.addEventListener('DOMContentLoaded', () => {
  initSidebarNavigation();
  initAttendanceRegister();
  initClassSessions();
  initStudentStatisticsFilter();
  initStudentSearchFilter();
  initAttendanceHistoryFilter();
  initReportExport();
});

/* =========================================================
   0. SIDEBAR NAVIGATION
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

  const navLinks = document.querySelectorAll('.role-nav .nav-item');
  const currentFile = decodeURIComponent(
    window.location.pathname.split('/').pop() || ''
  ).toLowerCase();

  navLinks.forEach((link) => {
    const label = (link.textContent || '')
      .replace(/\s+/g, ' ')
      .trim()
      .toLowerCase();

    const targetFile = routeMap[label];
    if (!targetFile) return;

    link.setAttribute('href', `./${targetFile}`);
    link.classList.toggle(
      'active',
      currentFile === targetFile.toLowerCase()
    );
  });
}

/* =========================================================
   1. MANUAL ATTENDANCE
   attendance-register.html
   ========================================================= */
function initAttendanceRegister() {
  const rows = [...document.querySelectorAll('.student-row')];

  if (!rows.length) return;

  const searchInput = document.getElementById('roster-search');
  const statusSelect = document.getElementById('status-filter');
  const shownCount = document.getElementById('shown-count');
  const savedBadge = document.querySelector('.badge-saved');
  const saveButton = document.getElementById('btn-save');

  const totalStudents = rows.length;

  function getRowStatus(row) {
    if (row.querySelector('.choice-btn.is-present')) return 'present';
    if (row.querySelector('.choice-btn.is-late')) return 'late';
    if (row.querySelector('.choice-btn.is-absent')) return 'absent';

    return 'none';
  }

  function updateSummary() {
    let present = 0;
    let late = 0;
    let absent = 0;

    rows.forEach((row) => {
      const status = getRowStatus(row);

      if (status === 'present') present++;
      if (status === 'late') late++;
      if (status === 'absent') absent++;
    });

    const values = document.querySelectorAll(
      '.summary-grid .stat-value'
    );

    if (values.length >= 4) {
      values[0].textContent = totalStudents;
      values[1].textContent = present;
      values[2].textContent = late;
      values[3].textContent = absent;
    }
  }

  function applyFilters() {
    const query = (searchInput?.value || '')
      .trim()
      .toLowerCase();

    const selectedStatus = (statusSelect?.value || 'all')
      .trim()
      .toLowerCase();

    let visibleCount = 0;

    rows.forEach((row) => {
      const rowText = row.textContent.toLowerCase();
      const rowStatus = getRowStatus(row);

      const matchSearch = !query || rowText.includes(query);
      const matchStatus =
        selectedStatus === 'all' ||
        selectedStatus === rowStatus;

      const isVisible = matchSearch && matchStatus;

      row.style.display = isVisible ? 'flex' : 'none';

      if (isVisible) visibleCount++;
    });

    if (shownCount) {
      shownCount.textContent =
        `Showing ${visibleCount} of ${totalStudents} students`;
    }
  }

  rows.forEach((row) => {
    const buttons = row.querySelectorAll('.choice-btn');

    buttons.forEach((button) => {
      button.addEventListener('click', () => {
        buttons.forEach((item) => {
          item.classList.remove(
            'is-present',
            'is-late',
            'is-absent'
          );
        });

        const status = button.textContent.trim().toLowerCase();

        if (status === 'present') {
          button.classList.add('is-present');
        } else if (status === 'late') {
          button.classList.add('is-late');
        } else if (status === 'absent') {
          button.classList.add('is-absent');
        }

        if (savedBadge) {
          savedBadge.textContent = 'Unsaved changes';
          savedBadge.classList.add('is-dirty');
        }

        updateSummary();
        applyFilters();
      });
    });
  });

  searchInput?.addEventListener('input', applyFilters);
  statusSelect?.addEventListener('change', applyFilters);

  saveButton?.addEventListener('click', () => {
    if (savedBadge) {
      savedBadge.textContent = 'Saved just now';
      savedBadge.classList.remove('is-dirty');
    }

    alert(
      'Attendance changes have been updated in the current page. Database saving requires API integration.'
    );
  });

  updateSummary();
  applyFilters();
}

/* =========================================================
   2. CLASS SESSION MANAGEMENT
   Class-Sessions.html
   ========================================================= */
function initClassSessions() {
  const sessionsCard = document.querySelector('.sessions-card');

  const submitButton = document.getElementById('btn-submit');
  const cancelButton = document.getElementById('btn-cancel');

  const dateInput = document.getElementById('f-date');
  const startInput = document.getElementById('f-start');
  const endInput = document.getElementById('f-end');
  const openInput = document.getElementById('f-open');
  const periodInput = document.getElementById('f-period');

  const autoClose = document.getElementById('auto-close');
  const errorMessage = document.getElementById('form-error');
  const editorTitle = document.getElementById('editor-title');

  if (!sessionsCard) return;

  let editingRow = null;

  function getSessionList() {
    const firstRow = sessionsCard.querySelector('.table-row');

    return firstRow ? firstRow.parentElement : sessionsCard;
  }

  function calculateAutoClose() {
    if (!periodInput || !autoClose) return;

    const match = periodInput.value.trim().match(/^(\d{1,2}):(\d{2})$/);

    if (!match) {
      autoClose.textContent = '--:--';
      return;
    }

    const totalMinutes =
      Number(match[1]) * 60 +
      Number(match[2]) +
      15;

    const hours = String(
      Math.floor(totalMinutes / 60) % 24
    ).padStart(2, '0');

    const minutes = String(totalMinutes % 60).padStart(2, '0');

    autoClose.textContent = `${hours}:${minutes}`;
  }

  function showError(message) {
    if (!errorMessage) return;

    errorMessage.textContent = message;
    errorMessage.hidden = Boolean(!message);
  }

  function formatDate(dateValue) {
    if (!dateValue) return '';

    const date = new Date(`${dateValue}T00:00:00`);

    if (Number.isNaN(date.getTime())) {
      return dateValue;
    }

    return date.toLocaleDateString('en-GB', {
      day: '2-digit',
      month: 'short',
      year: 'numeric'
    });
  }

  function getNextSessionId() {
    const existingIds = [
      ...sessionsCard.querySelectorAll('.col-session')
    ].map((element) => {
      const match = element.textContent.trim().match(/SE(\d+)/i);
      return match ? Number(match[1]) : 0;
    });

    const nextNumber = Math.max(0, ...existingIds) + 1;

    return `SE${String(nextNumber).padStart(3, '0')}`;
  }

  function resetEditor() {
    editingRow = null;

    if (dateInput) dateInput.value = '';
    if (startInput) startInput.value = '';
    if (endInput) endInput.value = '';
    if (openInput) openInput.value = '';
    if (periodInput) periodInput.value = '';

    if (editorTitle) {
      editorTitle.textContent = 'Create New Session';
    }

    showError('');
    calculateAutoClose();
  }

  function createSessionRow(sessionId, date, start, end, open, period) {
    const row = document.createElement('div');

    row.className = 'table-row';

    row.dataset.date = date;
    row.dataset.start = start;
    row.dataset.end = end;
    row.dataset.open = open;
    row.dataset.period = period;

    const sessionCell = document.createElement('div');
    sessionCell.className = 'col-session';
    sessionCell.textContent = sessionId;

    const scheduleCell = document.createElement('div');
    scheduleCell.className = 'col-schedule';
    scheduleCell.textContent =
      `${formatDate(date)} · ${start}–${end}`;

    const attendanceCell = document.createElement('div');
    attendanceCell.className = 'col-attendance';

    const statusBadge = document.createElement('span');
    statusBadge.className = 'badge badge-scheduled';
    statusBadge.textContent = 'Scheduled';

    attendanceCell.appendChild(statusBadge);

    const actionCell = document.createElement('div');
    actionCell.className = 'col-action';

    const actions = document.createElement('span');
    actions.className = 'row-actions';

    [
      ['view', 'View'],
      ['edit', 'Edit'],
      ['delete', 'Delete']
    ].forEach(([action, label], index) => {
      if (index > 0) {
        actions.appendChild(document.createTextNode(' / '));
      }

      const link = document.createElement('a');
      link.href = '#';
      link.className = 'action-link';
      link.dataset.action = action;
      link.textContent = label;

      actions.appendChild(link);
    });

    actionCell.appendChild(actions);

    row.append(
      sessionCell,
      scheduleCell,
      attendanceCell,
      actionCell
    );

    return row;
  }

  periodInput?.addEventListener('input', calculateAutoClose);

  submitButton?.addEventListener('click', (event) => {
    event.preventDefault();

    const date = dateInput?.value.trim() || '';
    const start = startInput?.value.trim() || '';
    const end = endInput?.value.trim() || '';
    const open = openInput?.value.trim() || '';
    const period = periodInput?.value.trim() || '';

    if (!date || !start || !end || !open || !period) {
      showError('Please complete all session fields.');
      return;
    }

    if (start >= end) {
      showError('End time must be later than start time.');
      return;
    }

    if (open > period) {
      showError(
        'Attendance opening time must be earlier than or equal to the attendance period end time.'
      );
      return;
    }

    showError('');

    if (editingRow) {
      editingRow.dataset.date = date;
      editingRow.dataset.start = start;
      editingRow.dataset.end = end;
      editingRow.dataset.open = open;
      editingRow.dataset.period = period;

      const scheduleCell = editingRow.querySelector('.col-schedule');

      if (scheduleCell) {
        scheduleCell.textContent =
          `${formatDate(date)} · ${start}–${end}`;
      }

      resetEditor();
      return;
    }

    const sessionId = getNextSessionId();

    const newRow = createSessionRow(
      sessionId,
      date,
      start,
      end,
      open,
      period
    );

    getSessionList().appendChild(newRow);

    resetEditor();
  });

  sessionsCard.addEventListener('click', (event) => {
    const link = event.target.closest('.action-link');

    if (!link) return;

    event.preventDefault();

    const row = link.closest('.table-row');
    if (!row) return;

    const action = link.dataset.action;

    if (action === 'delete') {
      if (confirm('Are you sure you want to delete this session?')) {
        row.remove();

        if (editingRow === row) {
          resetEditor();
        }
      }

      return;
    }

    if (action === 'edit') {
      editingRow = row;

      if (dateInput) dateInput.value = row.dataset.date || '';
      if (startInput) startInput.value = row.dataset.start || '';
      if (endInput) endInput.value = row.dataset.end || '';
      if (openInput) openInput.value = row.dataset.open || '';
      if (periodInput) periodInput.value = row.dataset.period || '';

      if (editorTitle) {
        editorTitle.textContent = 'Edit Class Session';
      }

      calculateAutoClose();
      showError('');

      return;
    }

    if (action === 'view') {
      const sessionId =
        row.querySelector('.col-session')?.textContent.trim() || '';

      const schedule =
        row.querySelector('.col-schedule')?.textContent.trim() || '';

      alert(`Session: ${sessionId}\nSchedule: ${schedule}`);
    }
  });

  cancelButton?.addEventListener('click', (event) => {
    event.preventDefault();
    resetEditor();
  });

  calculateAutoClose();
}

/* =========================================================
   3. STUDENT ATTENDANCE STATISTICS
   Student-statistics.html
   ========================================================= */
function initStudentStatisticsFilter() {
  const rows = [
    ...document.querySelectorAll('.stats-table-row')
  ];

  const summaryValues = document.querySelectorAll(
    '.stats-summary-grid .stat-value'
  );

  const summarySubs = document.querySelectorAll(
    '.stats-summary-grid .stat-sub'
  );

  const studentSelect = document.getElementById('student-filter');
  const rateSelect = document.getElementById('rate-filter');
  const emptyBox = document.getElementById('stats-empty');

  if (!rows.length) return;

  function selectStudent(row) {
    rows.forEach((item) => {
      item.classList.remove('is-selected');
    });

    row.classList.add('is-selected');

    const columns = row.children;

    if (columns.length < 5) return;

    const identity = columns[0].textContent.trim().split('·');

    const studentName = identity[0]?.trim() || '';
    const studentId = identity[1]?.trim() || '';

    const completed = columns[1].textContent.trim();
    const attended = columns[2].textContent.trim();
    const attendanceRate = columns[3].textContent.trim();
    const onTimeRate = columns[4].textContent.trim();

    if (summaryValues[0]) {
      summaryValues[0].textContent = studentName;
    }

    if (summarySubs[0]) {
      summarySubs[0].textContent = studentId;
    }

    if (summaryValues[1]) {
      summaryValues[1].textContent = attendanceRate;
    }

    if (summarySubs[1]) {
      summarySubs[1].textContent =
        `${attended} attended / ${completed} sessions`;
    }

    if (summaryValues[2]) {
      summaryValues[2].textContent = onTimeRate;
    }

    if (summarySubs[2]) {
      summarySubs[2].textContent =
        `On-time rate: ${onTimeRate}`;
    }
  }

  function applyFilters() {
    const selectedStudent = studentSelect?.value || 'all';
    const selectedRate = rateSelect?.value || 'all';

    let visibleCount = 0;

    rows.forEach((row) => {
      const columns = row.children;

      const identity = columns[0]?.textContent || '';
      const rateText = columns[3]?.textContent || '0';

      const rate = parseFloat(
        rateText.replace('%', '')
      ) || 0;

      const matchStudent =
        selectedStudent === 'all' ||
        identity.includes(selectedStudent);

      let matchRate = true;

      if (selectedRate === 'high') {
        matchRate = rate >= 90;
      } else if (selectedRate === 'mid') {
        matchRate = rate >= 75 && rate < 90;
      } else if (selectedRate === 'low') {
        matchRate = rate < 75;
      }

      const visible = matchStudent && matchRate;

      row.style.display = visible ? 'grid' : 'none';

      if (visible) visibleCount++;
    });

    if (emptyBox) {
      emptyBox.hidden = visibleCount !== 0;
    }

    const selectedRow = rows.find(
      (row) => row.style.display !== 'none'
    );

    if (selectedRow) {
      selectStudent(selectedRow);
    }
  }

  rows.forEach((row) => {
    row.addEventListener('click', () => {
      if (row.style.display !== 'none') {
        selectStudent(row);
      }
    });
  });

  studentSelect?.addEventListener('change', applyFilters);
  rateSelect?.addEventListener('change', applyFilters);

  applyFilters();
}

/* =========================================================
   4. STUDENT SEARCH & FILTERING
   studentsearch-filtering.html
   ========================================================= */
function initStudentSearchFilter() {
  const applyButton = document.getElementById('applyStudentFilters');
  const resetButton = document.getElementById('resetStudentFilters');

  const nameInput = document.getElementById('filterStudentName');
  const idInput = document.getElementById('filterStudentId');
  const courseInput = document.getElementById('filterCourse');

  const rows = [
    ...document.querySelectorAll('.students-table-row')
  ];

  const showingText = document.getElementById('studentShowingText');

  if (!rows.length) return;

  const totalStudents = rows.length;

  function runFilter() {
    const nameQuery = (nameInput?.value || '')
      .trim()
      .toLowerCase();

    const idQuery = (idInput?.value || '')
      .trim()
      .toLowerCase();

    const courseQuery = (courseInput?.value || '')
      .trim()
      .toLowerCase();

    let visibleCount = 0;

    rows.forEach((row) => {
      const name = (
        row.querySelector('.student-col-name')?.textContent || ''
      ).trim().toLowerCase();

      const id = (
        row.querySelector('.student-col-id')?.textContent || ''
      ).trim().toLowerCase();

      const course = (
        row.dataset.course || ''
      ).trim().toLowerCase();

      const matchName = !nameQuery || name.includes(nameQuery);
      const matchId = !idQuery || id.includes(idQuery);

      const matchCourse =
        !courseQuery ||
        course.includes(courseQuery) ||
        row.textContent.toLowerCase().includes(courseQuery);

      const visible = matchName && matchId && matchCourse;

      row.style.display = visible ? 'grid' : 'none';

      if (visible) visibleCount++;
    });

    if (showingText) {
      showingText.textContent =
        `Showing ${visibleCount} of ${totalStudents} students`;
    }
  }

  applyButton?.addEventListener('click', runFilter);

  nameInput?.addEventListener('input', runFilter);
  idInput?.addEventListener('input', runFilter);
  courseInput?.addEventListener('input', runFilter);

  [nameInput, idInput, courseInput].forEach((input) => {
    input?.addEventListener('keydown', (event) => {
      if (event.key === 'Enter') {
        runFilter();
      }
    });
  });

  resetButton?.addEventListener('click', () => {
    if (nameInput) nameInput.value = '';
    if (idInput) idInput.value = '';
    if (courseInput) courseInput.value = '';

    runFilter();
  });

  runFilter();
}

/* =========================================================
   5. ATTENDANCE HISTORY
   attendance-history.html
   ========================================================= */
function initAttendanceHistoryFilter() {
  const statusChip = document.getElementById('historyStatusChip');
  const studentChip = document.getElementById('historyStudentChip');

  const rows = [
    ...document.querySelectorAll('.history-table-row')
  ];

  const showingText = document.getElementById('historyShowingText');

  if (!rows.length) return;

  const totalRows = rows.length;

  const statusModes = [
    'All attendance statuses',
    'Present',
    'Late',
    'Absent'
  ];

  let statusIndex = 0;

  function applyStatusFilter() {
    const selectedStatus = statusModes[statusIndex];
    let visibleCount = 0;

    rows.forEach((row) => {
      const rowStatus = (
        row.dataset.status || ''
      ).trim().toLowerCase();

      const match =
        selectedStatus === 'All attendance statuses' ||
        rowStatus === selectedStatus.toLowerCase();

      row.style.display = match ? 'grid' : 'none';

      if (match) visibleCount++;
    });

    if (showingText) {
      showingText.textContent =
        `Showing ${visibleCount} of ${totalRows} sessions`;
    }
  }

  statusChip?.addEventListener('click', () => {
    statusIndex = (statusIndex + 1) % statusModes.length;

    statusChip.textContent = statusModes[statusIndex];

    applyStatusFilter();
  });

  /*
   * Lịch sử hiện tại đang hiển thị dữ liệu của:
   * Le Minh Khang - 079205003456
   * Database Systems - C0001
   *
   * Không tự động đổi sang sinh viên giả.
   */
  if (studentChip) {
    studentChip.textContent =
      'Le Minh Khang · 079205003456';
  }

  applyStatusFilter();
}

/* =========================================================
   6. ATTENDANCE REPORT EXPORT
   report-export.html
   ========================================================= */
function initReportExport() {
  const scopeInput = document.getElementById('reportScope');
  const formatInput = document.getElementById('reportFormat');

  const metaLine = document.getElementById('previewMetaLine');

  const exportButtons = document.querySelectorAll(
    '.export-pdf-btn'
  );

  const lecturerName = 'Nguyen Van An';
  const lecturerId = '079082001234';

  function updatePreview() {
    if (!metaLine) return;

    const scope = scopeInput?.value.trim() || 'All sessions';

    metaLine.textContent =
      `C0001 · ${scope} · Lecturer: ${lecturerName} (${lecturerId})`;
  }

  scopeInput?.addEventListener('input', updatePreview);

  exportButtons.forEach((button) => {
    button.addEventListener('click', () => {
      const format = formatInput?.value.trim() || 'PDF';

      if (format.toLowerCase() === 'pdf') {
        window.print();
      } else {
        alert(
          'This export format has not been implemented yet.'
        );
      }
    });
  });

  updatePreview();
}
