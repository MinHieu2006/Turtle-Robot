function H(side) {
    pendown();
    forward(side);
    penup();
    backward(side / 2);
    right(90);
    pendown();
    forward(side / 2);
    penup();
    left(90);
    pendown();
    forward(side / 2);
    backward(side);
}

function T(side) {
    pendown();
    forward(side);
    penup();
    right(90);
    pendown();
    forward(side / 2);
    backward(side);
    penup();
    forward(side / 2);
    left(90);
    backward(side);
}

function I(side) {
    pendown();
    forward(side);
    penup();
    backward(side);
}

function space(side) {
    penup();
    right(90);
    forward(side);
    left(90);
}

function A(side) {

    penup();
    right(30);
    pendown();
    forward(side);
    penup();
    right(120);
    pendown();
    forward(side);
    penup();
    backward(side / 2);
    right(120);
    pendown();
    forward(side / 2);
    penup();
    right(120);
    backward(side / 2);
    right(60);
    forward(5);
    left(90);


}

function L(side) {
    pendown();
    forward(side);
    penup();
    backward(side);
    right(90);
    pendown();
    forward(side / 2);
    penup();
    left(90);
}

function E(side) {
    penup();
    right(90);
    pendown();
    forward(side / 2);
    penup();
    backward(side / 2);
    left(90);
    pendown();
    forward(side / 2);

    penup();
    right(90);
    pendown();
    forward(side / 2);
    penup();
    backward(side / 2);
    left(90);
    pendown();
    forward(side / 2);

    penup();
    right(90);
    pendown();
    forward(side / 2);
    penup();
    backward(side / 2);
    left(90);
    backward(side);
    right(90);
    forward(side / 2);
    left(90);
}

function N(side) {
    pendown();
    forward(side);
    penup();
    backward(side);
    left(40);
    pendown();
    forward(Math.round(side * Math.sqrt(2)));
    penup();
    left(140);
    pendown();
    forward(side);
    penup();
    left(90);
    forward(side);
    left(90);
}

function U(side) {
    pendown();
    forward(side);
    penup();
    backward(side);
    right(90);
    pendown();
    forward(side / 2);
    penup();
    left(90);
    pendown();
    forward(side);
    penup();
    backward(side);
}

function V(side) {
    penup();
    left(30);
    pendown();
    forward(side);
    penup();
    backward(side);
    right(60);
    pendown();
    forward(side)
    penup();
    backward(side);
    right(60);
    forward(side);
    left(90);
}
