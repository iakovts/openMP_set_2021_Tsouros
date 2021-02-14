import os
import time

import numpy as np

from matplotlib import pyplot as plt
from matplotlib import animation, cm


class AnimateWave:
    def __init__(self, save_bool=False):
        self.inpt = np.fromfile(os.getcwd() + "/res1.txt", sep=" ")
        self.data = self.inpt.reshape(200, int(len(self.inpt) / 200))
        self.dt = 0.066978
        self.fig = plt.figure()
        self.ax = plt.axes(
            xlim=(0, self.data.shape[0]),
            ylim=(-1e11, 1e11),
        )
        self.x = np.arange(200)
        (self.p,) = self.ax.plot([], [], lw=2)
        
        self.sub_data = []
        self.pis = np.arange(5) * np.pi
        self.paused = False
        self.fig.canvas.mpl_connect("button_press_event", self.toggle_pause)
        self.save_bool = save_bool

    def __call__(self):
        self.anim = animation.FuncAnimation(
            self.fig,
            self.update,
            frames=self.data.shape[1],
            interval=60,
            blit=False,
            repeat=False,
            init_func=self._init_func
        )
        if self.save_bool:
            self.save_animation()

    def _init_func(self):
        return self.p
    
    def subplotting(self):
        sub_fig, sub_axes = plt.subplots(3, 2, sharex=True, sharey=True)
        for ax in sub_axes.flat:
            ax.set(xlabel="x", ylabel="$u(x,t)$")
        for idx, data in enumerate(self.sub_data):
            if idx < 3:
                sub_axes[idx, 0].plot(self.x, data)
                sub_axes[idx, 0].set_title(f"{idx}$\pi$")
            else:
                sub_axes[idx - 3, 1].plot(self.x, data)
                sub_axes[idx - 3, 1].set_title(f"{idx}$\pi$")

    def save_animation(self):
        self.anim.save(os.getcwd() + "/animationWave.gif")

    def check_pi(self, i):
        time_interval = self.dt * i
        pi_cond = np.isclose(time_interval, self.pis, atol=self.dt / 2)
        txt = f"{self.dt * i:.5}"
        is_pi = False
        if np.any(pi_cond):
            print(i)
            print("I m here")
            is_pi = True
            self.toggle_pause()
            idx = np.where(pi_cond == True)[0]
            if idx == 0:
                txt = "$0$"
            elif idx == 1:
                txt = "$\pi$"
            else:
                txt = f"{idx[0]}$\pi$"
        if i == self.data.shape[1] - 1:  # Last value should be closer to pi
            print("Or here")
            is_pi = True
            txt = "$5\pi$"
        return txt, is_pi

    def toggle_pause(self, *args, **kwargs):
        if self.paused:
            self.anim.event_source.start()
        else:
            self.anim.event_source.stop()
        self.paused = not self.paused

    def update(self, i):
        y = self.data[:, i]
        self.p.set_data(self.x, y)
        self.p.axes.set_ylim(-1.1, 1.1)
        self.p.axes.set_xlabel("$x$")
        self.p.axes.set_ylabel("$u(x, t)$")
        txt, is_pi = self.check_pi(i)
        if is_pi:
            self.sub_data.append(y)
        self.ax.set_title(f"Time {txt}")
        return self.p


def plot_3d_surf(data):
    fig, ax, = plt.subplots(subplot_kw={"projection": "3d"})
    x = np.arange(data.shape[0])
    y = np.arange(data.shape[1])
    X, Y = np.meshgrid(y, x)
    surf = ax.plot_surface(X, Y, data, cmap=cm.plasma, linewidth=0, antialiased=False)
    fig.colorbar(surf, shrink=0.5, aspect=5)
    plt.show()


if __name__ == "__main__":
    a = AnimateWave()
    a()
    plt.show()
