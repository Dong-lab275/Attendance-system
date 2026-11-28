document.addEventListener("DOMContentLoaded", () => {
    const currentPage = window.location.pathname
        .split("/")
        .pop()
        .toLowerCase();

    const navItems = document.querySelectorAll(
        ".sidebar-nav .nav-item"
    );

    navItems.forEach((item) => {
        const href = item.getAttribute("href");

        if (!href || href.startsWith("#")) {
            return;
        }

        const linkPage = href
            .split("#")[0]
            .split("/")
            .pop()
            .toLowerCase();

        if (linkPage === currentPage) {
            item.classList.add("active");
            item.setAttribute("aria-current", "page");
        } else {
            item.classList.remove("active");
            item.removeAttribute("aria-current");
        }
    });
});