
             import { PRICE } from "./config.js";
     
            function calcTotal() {
  
                const general = Number(document.getElementById("general").value);
                const student = Number(document.getElementById("student").value);
                const child = Number(document.getElementById("child").value);         

                const total = general * PRICE.general +
                              student *  PRICE.student +
                              child * PRICE.child;          

                document.getElementById("total").textContent = total.toLocaleString();


                    const btn = document.getElementById("purchaseBtn");

                 // ★ 合計金額でボタンの状態を切り替える
                 if (total > 0) {
                     btn.classList.remove("btn-disabled");
                     btn.classList.add("btn-active");
                 } else {
                     btn.classList.remove("btn-active");
                     btn.classList.add("btn-disabled");
                 }
                     // ★ 合計金額が更新されたら保存する
                 const purchaseData = { general, student, child };
                 localStorage.setItem("ticketPurchase", JSON.stringify(purchaseData));
            }         





document.addEventListener("DOMContentLoaded", () => {



    // ★ 以前の購入内容があれば復元する
    const saved = JSON.parse(localStorage.getItem("ticketPurchase"));
    if (saved) {
        document.getElementById("general").value = saved.general;
        document.getElementById("student").value = saved.student;
        document.getElementById("child").value = saved.child;

        // 合計金額も再計算
        calcTotal();
    }

            document.getElementById("general").addEventListener("input", calcTotal);
            document.getElementById("student").addEventListener("input", calcTotal);
            document.getElementById("child").addEventListener("input", calcTotal);
});



 document.getElementById("purchaseBtn").addEventListener("click", () => {
    const general = Number(document.getElementById("general").value);
    const student = Number(document.getElementById("student").value);
    const child = Number(document.getElementById("child").value);

    const total =
        general * PRICE.general +
        student * PRICE.student +
        child * PRICE.child;

    // ★ 合計金額が 0 以下なら確認ページへ行かない
    if (total <= 0) {
        return; // 処理を止める
    }

    // 購入内容を保存
    const purchaseData = { general, student, child };

    // ★ confirm に行くときだけ保存
    localStorage.setItem("ticketPurchase", JSON.stringify(purchaseData));
    // confirm.html に移動
    location.href = "confirm.html";
  });