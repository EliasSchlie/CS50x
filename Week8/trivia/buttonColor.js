function incorrect(b){
    b.style.backgroundColor = 'red'
    document.querySelector('p').innerHTML = 'incorrect';
};
function correct(b){
    b.style.backgroundColor = 'green'
    document.querySelector('p').innerHTML = 'correct';
};

let b0 = document.querySelector('#b0');
b0.addEventListener('click',function(){
	correct(b0);
});
let b3 = document.querySelector('#b3');
b3.addEventListener('click',function(){
	incorrect(b3);
});
let b5 = document.querySelector('#b5');
b5.addEventListener('click',function(){
	incorrect(b5);
});
let b8 = document.querySelector('#b8');
b8.addEventListener('click',function(){
	incorrect(b8);
});
let b10 = document.querySelector('#b10');
b10.addEventListener('click',function(){
	incorrect(b10);
});


/* 
document.querySelector('button').style.color = red;

button.addEventListener('click', function () {
    button.style.color = red;
});
*/