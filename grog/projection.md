# Projection in grog
$$
pos_{world_{10}}=
\left(\begin{array}{cc}
x_{10}\\
y_{10}\\
z_{10}\\
\end{array}\right)
$$

$$
Proj_{scale_{10}} =  \left(\begin{array}{cc}
f_{x_{10}} * 40 & 0 & 0 & 0\\
0 & f_{y_{10}} * 32 & 0 & 0\\
0 & 0 & \left(\frac{far  +  near}{near  -  far}\right)_{10} & \left(\frac{2  *  far  *  near}{near  -  far}\right)_{10}\\
0 & 0& -1 & 0
\end{array}\right)
$$

$$
MVP_{10} = Model * View * Proj_{scale_{10}}
$$

$$
pos_{proj_{20}} = MVP_{10} * \left(\begin{array}{cc} pos_{world_{10}} \\ 1_{10}\end{array}\right) = \left(\begin{array}{cc}
X_{scale_{20}}\\
Y_{scale_{20}}\\
Z_{scale_{20}}\\
W_{scale_{20}}\\
\end{array}\right)
$$

Closer than near means $Z/W < -1$, ie $Z_{scale_{20}} < -W_{scale_{20}}$.
Take $P1_{proj_{20}} = \left(\begin{array}{cc}
X1_{scale_{20}}\\
Y1_{scale_{20}}\\
Z1_{scale_{20}}\\
W1_{scale_{20}}\\
\end{array}\right)$, simplified as $P1 = \left(\begin{array}{cc}
X1\\
Y1\\
Z1\\
W1\\
\end{array}\right)$ and $P2 = \left(\begin{array}{cc}
X2\\
Y2\\
Z2\\
W2\\
\end{array}\right)$.
If $P1$ and $P2$ are accross the $near$ plane, look for point $Pn$ on the $(P1,P2)$ segment crossing the $near$ plane, ie $(1) Zn=-Wn$.
$\gamma$ such that $\gamma * P1 + (1-\gamma) * P2 = Pn$.
$(2) Zn = \gamma * Z1 + (1-\gamma) * Z2 = \gamma * (Z1 - Z2) + Z2$
$(3) Wn = \gamma * W1 + (1-\gamma) * W2 = \gamma * (W1 - W2) + W2$
Thus $(2) = -(3)$
$\gamma * (Z1 - Z2 - W1 + W2) = Z2 - W2$
$\gamma = \frac{Z2 - W2}{Z1 - Z2 - W1 + W2}$

$pos_{proj_{20}}$ is transformed for screen through the following steps:

 - $W_{10} = W_{scale_{20}} >> 10$
 - $X_{screen} = \frac{X_{scale_{20}}}{W_{10}} >> 10 + 40$
 - $Y_{screen} = \frac{Y_{scale_{20}}}{W_{10}} >> 10 + 32$
