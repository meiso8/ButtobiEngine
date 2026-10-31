document.addEventListener("DOMContentLoaded", () => {
    document.querySelectorAll("a, button").forEach(el => {
        el.addEventListener("click", (e) => {
            const href = el.getAttribute("href");
            // 保存内容を取得
            const saved = JSON.parse(localStorage.getItem("ticketPurchase"));

            // ★ index.html に行こうとした時だけ警告
            if (href === "index.html") {

                // ★ 保存内容がない or 合計が 0 → 警告なしで削除して移動
                if (!saved || (
                    saved.general == 0 &&
                    saved.student == 0 &&
                    saved.child == 0
                )) {
                    localStorage.removeItem("ticketPurchase");
                    return; // そのまま移動
                }

                const result = confirm("変更内容が破棄されます。よろしいですか？");

                if (!result) {
                    e.preventDefault(); // キャンセル → 移動しない
                } else {
                    localStorage.removeItem("ticketPurchase"); // OK → 削除
                }
            }
        });
    });
});
