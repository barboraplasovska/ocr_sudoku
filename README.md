# Neural network to recognise digits

## How to run it?

1. ```make main```
2. ```./main```

## Training

- to train with the **MNIST database** of handwritten digits uncomment *trainWithMnist* and *testWithMnist* in the main function.
- to train with **computer generated digits**, uncomment *train* and *test* in the main function.
- after training, weights and biases of the trained model will be saved in the **weights.txt** file, so later you can only load the weights from the file using *loadWeights* function and test your network.