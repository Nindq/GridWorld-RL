import numpy as np
import matplotlib.pyplot as plt

def plot_training_metrics(episode_steps, episode_rewards, epsilons, window_size=5,
                          save_path="gridworld_performance.png"):
    """Plot steps, total rewards, and epsilon decay progression across episodes."""
    moving_avg_steps = np.convolve(
        episode_steps, np.ones(window_size) / window_size, mode='valid'
    )

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 10))

    # Plot 1: Steps & Moving Average
    ax1.plot(episode_steps, alpha=0.4, label="Raw Steps")
    ax1.plot(range(window_size - 1, len(episode_steps)),
             moving_avg_steps, color='red', label=f"Moving Avg ({window_size} ep)")
    ax1.set_title("Agent Convergence: Steps per Episode")
    ax1.set_ylabel("Steps")
    ax1.legend()
    ax1.grid(True)

    # Plot 2: Total Rewards
    ax2.plot(episode_rewards, color='green', label="Total Reward")
    ax2.set_title("Reward Progression per Episode")
    ax2.set_ylabel("Reward")
    ax2.legend()
    ax2.grid(True)

    # Plot 3: Epsilon Decay
    ax3.plot(epsilons, color='purple', label="Epsilon (Exploration Rate)")
    ax3.set_title("Epsilon Decay over Episodes")
    ax3.set_xlabel("Episode")
    ax3.set_ylabel("Epsilon")
    ax3.legend()
    ax3.grid(True)

    plt.tight_layout()
    plt.savefig(save_path)
    plt.show()