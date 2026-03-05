document.addEventListener("DOMContentLoaded", function () {

    const form = document.getElementById("orderForm");
    const message = document.getElementById("formMessage");

    form.addEventListener("submit", async function (e) {

        e.preventDefault();

        const name = form.querySelector('input[name="name"]').value.trim();
        const item = form.querySelector('input[name="item"]').value.trim();
        const address = form.querySelector('input[name="address"]').value.trim();

        if (!name || !item || !address) {
            message.style.color = "red";
            message.textContent = "Please fill all fields.";
            return;
        }

        message.style.color = "#0f0";
        message.textContent = "Processing order...";

        const response = await fetch("/order", {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: `name=${encodeURIComponent(name)}&item=${encodeURIComponent(item)}&address=${encodeURIComponent(address)}`
        });

        const data = await response.json();
        message.textContent = data.message;
    });

});