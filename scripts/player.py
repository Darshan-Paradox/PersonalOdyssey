def get_level(stats, weights):

    for i in zip(stats, weights):
        rms = weight*stats**2
    rms = np.sqrt(rms/len(stats))

    return np.ln(rms)

def get_academic(tasks):
    return level*level_effect*np.ln(tasks)

def get_critic(tasks):
    bias = 1
    slope = 0.05
    return level*level_effect*(-np.exp(-tasks) + bias + slope*tasks)

def get_creativity(tasks):
    return level*level_effect*0.02*tasks*np.ln(tasks)

def get_coins(coding, academics, physical, surprise, streak):

    coins = coding + (academics + physical)/2 + surprise*2

    if coding == 0:
        coins = coins - 1
    if academics == 0:
        coins = coins - 0.5
    if physical == 0:
        coins = coins - 0.5

    if coding == academics and academics == physical and coding == 0:
        coins = coins - 1

    bias = 1
    slope = 0.03
    coins = coins + -np.exp(-streak) + bias + slope*streak;

    return level*level_effect*coins
