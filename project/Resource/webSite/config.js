
export const PRICE = {
  general: 2000,
  student: 1500,
  child: 600
};

document.addEventListener("DOMContentLoaded", () => {
document.querySelectorAll(".price-general").forEach(el => {
  el.textContent = PRICE.general;
});

document.querySelectorAll(".price-student").forEach(el => {
  el.textContent = PRICE.student;
});

document.querySelectorAll(".price-child").forEach(el => {
  el.textContent = PRICE.child;
});
});
