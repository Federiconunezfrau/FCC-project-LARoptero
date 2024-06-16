import numpy as np

def flip_bytes(x,length,stringByteOrder="little"):
    aux = (x.to_bytes(length,byteorder=stringByteOrder))
    if(stringByteOrder == "little"):
        auxStringByteOrder = "big"
    else:
        auxStringByteOrder = "little"
    return int.from_bytes(aux, byteorder=auxStringByteOrder)

def mean_and_std(x):

    x_aux = []

    mu = np.mean(x)
    sigma = np.std(x)

    for data in x:
        if ( data < (mu - 6*sigma) ) or ( data > (mu + 6*sigma) ):
            x_aux.append(x)

    mu = np.mean(x_aux)
    sigma = np.std(x_aux)

    return mu, sigma