# Fast Matrix Multiplication

_to view the flowcharts properly, please use [FieryRMS/vscode-markdown-mermaid/v1.22.0](https://github.com/FieryRMS/vscode-markdown-mermaid/releases/tag/v1.22.0). As of 30/03/2024, mermaid does not reliably support mathmatical formula rendering for all platforms._

## Naive Matrix Multiplication

```mermaid
flowchart TD
    A(["$$\text{Naive}\left(A,B\right)$$"]) --> B("$$A\times B = \begin{bmatrix} a_{11} & a_{12} & a_{13} & \cdots & a_{1n} \\ a_{21} & a_{22} & a_{23} & \cdots & a_{2n} \\ a_{31} & a_{32} & a_{33} & \cdots & a_{3n} \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ a_{m1} & a_{m2} & a_{m3} & \cdots & a_{mn} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} & b_{13} & \cdots & b_{1p} \\ b_{21} & b_{22} & b_{23} & \cdots & b_{2p} \\ b_{31} & b_{32} & b_{33} & \cdots & b_{3p} \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ b_{n1} & b_{n2} & b_{n3} & \cdots & b_{np} \end{bmatrix}$$")

    B --> C["$$c_{ij} = \sum_{k=1}^{n} a_{ik}b_{kj}$$"]

    C --> D["$$= \begin{bmatrix} c_{11} & c_{12} & c_{13} & \cdots & c_{1p} \\ c_{21} & c_{22} & c_{23} & \cdots & c_{2p} \\ c_{31} & c_{32} & c_{33} & \cdots & c_{3p} \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ c_{m1} & c_{m2} & c_{m3} & \cdots & c_{mp} \end{bmatrix}$$"]

    D --> E([Return])
```

## The Strassen algorithm

```mermaid
flowchart TD
    A(["$$\text{Strassen}\left(A,B\right)$$"]) --> BB{"Is the Matrices 1 x 1?"}
    BB -- Yes --> CC["$$C = \left[a_{11} \times b_{11}\right]$$"]
    CC --> DD([Return])
    BB -- No --> B{{"Partition the Matrix"}}
    B --> C["$$A \times B = \left[ \begin{array}{c|c} A_{11} & A_{12}  \\ \hline A_{21} & A_{22} \end{array} \right] \times \left[ \begin{array}{c|c} B_{11} & B_{12}  \\ \hline B_{21} & B_{22} \end{array} \right]$$"]
    C --> D["$$P_1 = A_{11}\left(B_{12} - B_{22}\right) \\ P_2 = \left(A_{11} + A_{12}\right)B_{22} \\ P_3 = \left(A_{21} + A_{22}\right)B_{11} \\ P_4 = A_{22}\left(B_{21} - B_{11}\right) \\ P_5 = \left(A_{11} + A_{22}\right)\left(B_{11} + B_{22}\right) \\ P_6 = \left(A_{12} - A_{22}\right)\left(B_{21} + B_{22}\right) \\ P_7 = \left(A_{11} - A_{21}\right)\left(B_{11} + B_{12}\right)$$"]

    D --> EE[["$$\text{For each, use Strassen}\left(A_{i,j},B_{k,l}\right)$$"]]

    EE --> E["$$C_{11} = P_5 + P_4 - P_2 + P_6 \\ C_{12} = P_1 + P_2 \\ C_{21} = P_3 + P_4 \\ C_{22} = P_5 + P_1 - P_3 - P_7$$"]

    E --> F["$$= \begin{bmatrix} C_{11} & C_{12} \\ C_{21} & C_{22} \end{bmatrix} = C$$"]

    F --> G([Return])
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
    A(["$$\text{MUL}\left(A,B\right)$$"]) --> B["$$A\times B = \begin{bmatrix} a_{11} & a_{12} & \cdots & a_{1n} \\ a_{21} & a_{22} & \cdots & a_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ a_{n1} & a_{n2} & \cdots & a_{nn} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} & \cdots & b_{1n} \\ b_{21} & b_{22} & \cdots & b_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ b_{n1} & b_{n2} & \cdots & b_{nn} \end{bmatrix}$$"]
    B --> C{"Is the Matrix 2 x 2?"}
    C -- No --> D{{"Partition the Matrix"}}
    D --> E["$$A \times B = \left[ \begin{array}{c|c} A_{11} & A_{12}  \\ \hline A_{21} & A_{22} \end{array} \right] \times \left[ \begin{array}{c|c} B_{11} & B_{12}  \\ \hline B_{21} & B_{22} \end{array} \right]$$"]
    E --> F["$$= \left[ \begin{array}{cc} A_{11}B_{11} + A_{12}B_{21} & A_{11}B_{12} + A_{12}B_{22} \\ A_{21}B_{11} + A_{22}B_{21} & A_{21}B_{12} + A_{22}B_{22} \end{array} \right]$$"]
    F --> G["$$= \left[ \begin{array}{cc} \text{MUL}\left(A_{11},B_{11}\right) + \text{MUL}\left(A_{12},B_{21}\right) & \text{MUL}\left(A_{11},B_{12}\right) + \text{MUL}\left(A_{12},B_{22}\right) \\ \text{MUL}\left(A_{21},B_{11}\right) + \text{MUL}\left(A_{22},B_{21}\right) & \text{MUL}\left(A_{21},B_{12}\right) + \text{MUL}\left(A_{22},B_{22}\right) \end{array} \right]$$"]
    G --> H(["Return"])
    C -- Yes --> I[["$$\text{Strassen}\left(A,B\right)$$"]]
    I --> J(["Return"])
```

## Winograd's Algorithm (1968)

```mermaid
flowchart LR
    A(["$$\text{Winograd}\left(A,B\right)$$"]) --> B["$$A\times B = \begin{bmatrix} a_{11} & a_{12} & \cdots & a_{1n} \\ a_{21} & a_{22} & \cdots & a_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ a_{m1} & a_{m2} & \cdots & a_{mn} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} & \cdots & b_{1p} \\ b_{21} & b_{22} & \cdots & b_{2p} \\ \vdots & \vdots & \ddots & \vdots \\ b_{n1} & b_{n2} & \cdots & b_{np} \end{bmatrix}$$"]
    B --> CC{{"Preproccessing"}}
    CC --> C["$$D_i = \sum_{k=1}^{n/2}a_{i,2k-1} \cdot a_{i,2k} \\ E_j =\sum_{k=1}^{n/2}b_{2k-1,j} \cdot a_{2k,j}$$"]
    C --> DD{"Is n even?"}
    DD -- Yes --> D["$$C_{i,j} = \sum^{n/2}_{k=1}(a_{i,2k-1}+b_{2k,j})(a_{i,2k}+b_{2k-1,j})-D_i-E_j$$"]
    DD -- No --> F["$$C_{i,j} = \sum^{n/2}_{k=1}(a_{i,2k-1}+b_{2k,j})(a_{i,2k}+b_{2k-1,j})-D_i-E_j+\boldsymbol{ a_{i,n}b_{n,j} }$$"]
    D --> E(["Return"])
    F --> E
```

This algorithm is asymptotically the same as the naive algorithm, but it has a lower constant factor. It trades off the number of multiplications for the number of additions. The time complexity of the Winograd algorithm is $O(n^3)$.

## Divide and Conquer

```mermaid
flowchart TD
    A(["$$\text{DNC}\left(A,B\right)$$"]) --> B["$$A\times B = \begin{bmatrix} a_{11} & a_{12} & \cdots & a_{1n} \\ a_{21} & a_{22} & \cdots & a_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ a_{m1} & a_{m2} & \cdots & a_{mn} \end{bmatrix} \times \begin{bmatrix} b_{11} & b_{12} & \cdots & b_{1p} \\ b_{21} & b_{22} & \cdots & b_{2p} \\ \vdots & \vdots & \ddots & \vdots \\ b_{n1} & b_{n2} & \cdots & b_{np} \end{bmatrix}$$"]

    B --> CC{"$$\text{is }A, B \text{ small?}$$"}

    CC -- No --> C{"$$\text{is }max(m,n,p)=m$$?"}

    C -- Yes --> D{{"$$\text{Split }A\text{ horizontally}$$"}}

    D --> E["$$C = A \times B = \left[ \begin{array}{c} A_{1}  \\ \hline A_{2} \end{array} \right] \times B = \left[ \begin{array}{c} A_{1}B  \\ A_{2}B \end{array} \right]$$"]

    E --> F["$$= \left[ \begin{array}{c} \text{DNC}\left(A_{1},B\right)  \\ \text{DNC}\left(A_{2},B\right) \end{array} \right]$$"]

    F --> L

    C -- No --> H{"$$\text{is }max(m,n,p)=p$$?"}

    H -- Yes --> I{{"$$\text{Split }B\text{ vertically}$$"}}

    I --> J["$$C = A \times B = A \times \left[ \begin{array}{c|c} B_{1}  & B_{2} \end{array} \right] = \left[ \begin{array}{cc} A B_{1} & A B_{2} \end{array} \right]$$"]

    J --> K["$$= \left[ \begin{array}{cc} \text{DNC}\left(A,B_{1}\right) & \text{DNC}\left(A,B_{2}\right) \end{array} \right]$$"]

    K --> L(["Return"])

    H -- No --> M{{"$$\text{Split }A\text{ vertically and }B\text{ horizontally}$$"}}

    M --> N["$$C = A \times B = \left[ \begin{array}{c|c} A_{1} & A_{2} \end{array} \right] \times \left[ \begin{array}{c} B_{1}  \\ \hline B_{2} \end{array} \right] = \begin{array}{c} A_{1}B_{1} + A_{2}B_{2} \end{array} $$"]

    N --> O["$$= \text{DNC}\left(A_{1},B_{1}\right) + \text{DNC}\left(A_{2},B_{2}\right)$$"]

    O --> L

    CC -- Yes --> Q{{"Simple unrolled loop for multiplication"}}

    Q --> R(["Return"])

```
