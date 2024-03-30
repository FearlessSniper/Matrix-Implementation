# Fast Matrix Multiplication

_to view the flowcharts properly, please use [FieryRMS/vscode-markdown-mermaid/v1.22.0](https://github.com/FieryRMS/vscode-markdown-mermaid/releases/tag/v1.22.0). As of 30/03/2024, mermaid does not reliably support mathmatical formula rendering for all platforms._

## Naive Matrix Multiplication

```mermaid
flowchart TD
    A["$$\text{Naive}\left(A,B\right)$$"] --> B("$$A\times B = \begin{bmatrix} a_{11} & a_{12} & a_{13} & \cdots & a_{1n} \\ a_{21} & a_{22} & a_{23} & \cdots & a_{2n} \\ a_{31} & a_{32} & a_{33} & \cdots & a_{3n} \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ a_{m1} & a_{m2} & a_{m3} & \cdots & a_{mn} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} & b_{13} & \cdots & b_{1p} \\ b_{21} & b_{22} & b_{23} & \cdots & b_{2p} \\ b_{31} & b_{32} & b_{33} & \cdots & b_{3p} \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ b_{n1} & b_{n2} & b_{n3} & \cdots & b_{np} \end{bmatrix}$$")

    B --> C["$$c_{ij} = \sum_{k=1}^{n} a_{ik}b_{kj}$$"]

    C --> D["$$= \begin{bmatrix} c_{11} & c_{12} & c_{13} & \cdots & c_{1p} \\ c_{21} & c_{22} & c_{23} & \cdots & c_{2p} \\ c_{31} & c_{32} & c_{33} & \cdots & c_{3p} \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ c_{m1} & c_{m2} & c_{m3} & \cdots & c_{mp} \end{bmatrix}$$"]

    D --> E[Return]
```

## The Strassen algorithm

```mermaid
flowchart TD
    A["$$\text{Strassen}\left(A,B\right)$$"] --> B("$$A\times B = \begin{bmatrix} a_{11} & a_{12} \\ a_{21} & a_{22} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} \\ b_{21} & b_{22} \end{bmatrix}$$")
    B --> C["$$P_1 = a_{11}\left(b_{12} - b_{22}\right) \\ P_2 = \left(a_{11} + a_{12}\right)b_{22} \\ P_3 = \left(a_{21} + a_{22}\right)b_{11} \\ P_4 = a_{22}\left(b_{21} - b_{11}\right) \\ P_5 = \left(a_{11} + a_{22}\right)\left(b_{11} + b_{22}\right) \\ P_6 = \left(a_{12} - a_{22}\right)\left(b_{21} + b_{22}\right) \\ P_7 = \left(a_{11} - a_{21}\right)\left(b_{11} + b_{12}\right)$$"]

    C --> D["$$c_{11} = P_5 + P_4 - P_2 + P_6 \\ c_{12} = P_1 + P_2 \\ c_{21} = P_3 + P_4 \\ c_{22} = P_5 + P_1 - P_3 - P_7$$"]

    D --> E["$$= \begin{bmatrix} c_{11} & c_{12} \\ c_{21} & c_{22} \end{bmatrix}$$"]

    E --> F[Return]
```

### Tensor decomposition of Strassen algorithm

![Tensor Decompostion Created on ](./assets/Figma_matrix_decomposition.png)
[image inspired from wiki](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2e/Strassen_algorithm.svg/1200px-Strassen_algorithm.svg.png)


This method uses only 7 multiplications instead of 8. The Strassen algorithm is faster than the naive algorithm for large matrices. The time complexity of the Strassen algorithm is $O(n^{\log_2 7}) \approx O(n^{2.81})$ compared to the naive algorithm which has a time complexity of $O(n^3)$.

It has been proven that a 2x2 matrix multiplication cannot be done in fewer than 7 multiplications. The Strassen algorithm is optimal in this sense.

## Recursive Matrix Multiplication

For now, we assume that the matrix is a square matrix and the size of the matrix is a power of 2.

```mermaid
flowchart TD
    A(["$$\text{MUL}\left(A,B\right)$$"]) --> B["$$A\times B = \begin{bmatrix} a_{11} & a_{12} & \cdots & a_{1n} \\ a_{21} & a_{22} & \cdots & a_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ a_{m1} & a_{m2} & \cdots & a_{mn} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} & \cdots & b_{1n} \\ b_{21} & b_{22} & \cdots & b_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ b_{m1} & b_{m2} & \cdots & b_{mn} \end{bmatrix}$$"]
    B --> C{"Is the Matrix 2 x 2?"}
    C -- No --> D{{"Partition the Matrix"}}
    D --> E["$$A \times B = \left[ \begin{array}{c|c} A_{11} & A_{12}  \\ \hline A_{21} & A_{22} \end{array} \right] \times \left[ \begin{array}{c|c} B_{11} & B_{12}  \\ \hline B_{21} & B_{22} \end{array} \right]$$"]
    E --> F["$$= \left[ \begin{array}{cc} A_{11}B_{11} + A_{12}B_{21} & A_{11}B_{12} + A_{12}B_{22} \\ A_{21}B_{11} + A_{22}B_{21} & A_{21}B_{12} + A_{22}B_{22} \end{array} \right]$$"]
    F --> G["$$= \left[ \begin{array}{cc} \text{MUL}\left(A_{11},B_{11}\right) + \text{MUL}\left(A_{12},B_{21}\right) & \text{MUL}\left(A_{11},B_{12}\right) + \text{MUL}\left(A_{12},B_{22}\right) \\ \text{MUL}\left(A_{21},B_{11}\right) + \text{MUL}\left(A_{22},B_{21}\right) & \text{MUL}\left(A_{21},B_{12}\right) + \text{MUL}\left(A_{22},B_{22}\right) \end{array} \right]$$"]
    G --> H(["Return"])
    C -- Yes --> I[["$$\text{Strassen}\left(A,B\right)$$"]]
    I --> J(["Return"])
```
