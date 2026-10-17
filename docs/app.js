document.addEventListener('DOMContentLoaded', () => {
  // Mobile drawer toggle
  const menuToggle = document.querySelector('.menu-toggle');
  const sidebar = document.querySelector('.sidebar');

  if (menuToggle && sidebar) {
    menuToggle.addEventListener('click', () => {
      sidebar.classList.toggle('open');
    });

    sidebar.querySelectorAll('a').forEach(link => {
      link.addEventListener('click', () => {
        sidebar.classList.remove('open');
      });
    });
  }

  // Copy buttons for code blocks
  document.querySelectorAll('.copy-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const wrapper = btn.closest('.code-wrapper');
      if (wrapper === null) return;
      const code = wrapper.querySelector('code').innerText;
      navigator.clipboard.writeText(code).then(() => {
        const originalText = btn.innerText;
        btn.innerText = 'Copied';
        btn.style.color = '#10b981';
        setTimeout(() => {
          btn.innerText = originalText;
          btn.style.color = '';
        }, 2000);
      });
    });
  });

  // Sidebar navigation search filter
  const searchInput = document.getElementById('docs-search');
  if (searchInput) {
    searchInput.addEventListener('input', (e) => {
      const query = e.target.value.toLowerCase().trim();
      const navItems = document.querySelectorAll('.nav-items li');

      navItems.forEach(item => {
        const text = item.textContent.toLowerCase();
        item.style.display = text.includes(query) ? '' : 'none';
      });
    });
  }

  // Interactive 18 Templates search and category filter
  const templateSearch = document.getElementById('template-search');
  const filterBtns = document.querySelectorAll('.filter-btn');
  const templateRows = document.querySelectorAll('#templates-table tbody tr');
  let activeCategory = 'all';

  function filterTemplates() {
    const query = templateSearch ? templateSearch.value.toLowerCase().trim() : '';

    templateRows.forEach(row => {
      const category = row.getAttribute('data-category') || '';
      const text = row.textContent.toLowerCase();
      const matchesCategory = activeCategory === 'all' || category.includes(activeCategory);
      const matchesQuery = query === '' || text.includes(query);

      row.style.display = (matchesCategory && matchesQuery) ? '' : 'none';
    });
  }

  if (templateSearch) {
    templateSearch.addEventListener('input', filterTemplates);
  }

  filterBtns.forEach(btn => {
    btn.addEventListener('click', () => {
      filterBtns.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      activeCategory = btn.getAttribute('data-filter') || 'all';
      filterTemplates();
    });
  });

  // Scroll spy for active sidebar link
  const sections = document.querySelectorAll('section');
  const navLinks = document.querySelectorAll('.nav-items a');

  window.addEventListener('scroll', () => {
    let current = '';
    const scrollPos = window.scrollY + 120;

    sections.forEach(section => {
      if (section.offsetTop <= scrollPos) {
        current = section.getAttribute('id');
      }
    });

    navLinks.forEach(link => {
      link.classList.remove('active');
      if (link.getAttribute('href') === `#${current}`) {
        link.classList.add('active');
      }
    });
  });
});
