let form = document.querySelector('form');
form.addEventListener('submit', function(event){
    answer = document.querySelector('#answer')
    event.preventDefault();
    if (answer.value == 'B-cell' || answer.value == 'B-cells'){
        document.querySelector('#q2').innerHTML = 'correct';
        answer.style.backgroundColor = 'green';
    } else {
        document.querySelector('#q2').innerHTML = 'incorrect';
        answer.style.backgroundColor = 'red';    
    }
});