  // 画面内に要素が入ってきたかを監視する処理
  const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
      if (entry.isIntersecting) {
        // 画面内に入ったら is-show クラスを追加
        entry.target.classList.add('is-show');
      }
    });
  }, {
    threshold: 0.2 // 要素が画面に20%見えたら実行
  });

document.addEventListener('DOMContentLoaded', () => {
  document.querySelectorAll('.fade-in').forEach(el => observer.observe(el));
});