// =============================================
// Red Hen Kitchen Frontend Script
// =============================================

document.addEventListener("DOMContentLoaded", function () {

    const form = document.getElementById("orderForm");
    const message = document.getElementById("formMessage");

    form.addEventListener("submit", function (e) {

        const name = form.elements["name"].value.trim();
        const item = form.elements["item"].value.trim();
        const address = form.elements["address"].value.trim();

        if (name.length < 2 || item.length < 2 || address.length < 5) {
            e.preventDefault();
            message.style.color = "red";
            message.textContent = "Please fill all fields correctly.";
            return;
        }

        message.style.color = "#0f0";
        message.textContent = "Processing order...";
    });

});