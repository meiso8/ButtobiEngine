/*const a = 3;
const b = 5;
let c = "です";
alert(a+b+c);
*/

const disp = document.getElementById("second");
const kakugen_items = ["格言1","格言2","格言3"];

const btn = document.getElementById("btn")
btn.addEventListener("click",function(){
   const num  = Math.floor(Math.random()*kakugen_items.length);
   disp.innerHTML = kakugen_items[num];
})
