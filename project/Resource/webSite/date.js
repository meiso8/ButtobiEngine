function formatDate(date) {
    const y = date.getFullYear();
    const m = String(date.getMonth() + 1).padStart(2, "0");
    const d = String(date.getDate()).padStart(2, "0");
    return `${y}年${m}月${d}日`;
}

const num = 100;

document.addEventListener("DOMContentLoaded", () => {
    const start = new Date();               // 今日
    const end = new Date();                 // 終了日用
    end.setDate(start.getDate() +num);      // 100日後

    const startStr = formatDate(start);
    const endStr = formatDate(end);

    document.getElementById("event-period").textContent =
        `開催期間：${startStr} ～ ${endStr}（${num}日間）`;
});
