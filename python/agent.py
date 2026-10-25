import random
import numpy as np
import gridworld_module
import torch
import torch.nn as nn
from collections import deque


class QLearningAgent:
    """Q-table management, epsilon-greedy action selection, and Q-value updates."""

    ACTIONS = [
        gridworld_module.Action.UP,
        gridworld_module.Action.DOWN,
        gridworld_module.Action.RIGHT,
        gridworld_module.Action.LEFT,
    ]

    def __init__(self, grid_size, discount=0.9, learning_rate=0.2,
                 epsilon=1.0, epsilon_decay=0.93, min_epsilon=0.01):
        self.discount = discount
        self.learning_rate = learning_rate
        self.epsilon = epsilon
        self.epsilon_decay = epsilon_decay
        self.min_epsilon = min_epsilon

        self.q_table = np.zeros((grid_size ** 2, len(self.ACTIONS)))

    def choose_action(self, state_idx):
        """Epsilon-greedy action selection."""
        if random.random() < self.epsilon:
            return random.choice(self.ACTIONS)
        return self.ACTIONS[np.argmax(self.q_table[state_idx])]

    def update_q_value(self, state_idx, next_state_idx, action_idx, reward, done=False):
        """Standard Q-learning update (off-policy TD)."""
        target = reward + (1 - int(done)) * self.discount * np.max(self.q_table[next_state_idx])
        old_val = self.q_table[state_idx, action_idx]
        self.q_table[state_idx, action_idx] = old_val + self.learning_rate * (target - old_val)

    def decay_epsilon(self):
        self.epsilon = max(self.epsilon * self.epsilon_decay, self.min_epsilon)

    def best_action(self, state_idx):
        """Greedy action — used for the final demonstration run."""
        return self.ACTIONS[np.argmax(self.q_table[state_idx])]

class ReplayBuffer:
    def __init__(self, max_size=10000):
        self.max_size = max_size
        self.buffer = deque(maxlen=max_size)
    
    def append(self, state, action, reward, next_state, done):
        self.buffer.append((state, action, reward, next_state, done))
    
    def sample(self, batch_size):
        samples = random.sample(self.buffer, batch_size)
        
        states, actions, rewards, next_states, dones = zip(*samples)
        
        return (
            torch.tensor(states, dtype=torch.float32),
            torch.tensor(actions, dtype=torch.long),
            torch.tensor(rewards, dtype=torch.float32),
            torch.tensor(next_states, dtype=torch.float32),
            torch.tensor(dones, dtype=torch.float32),
        )
    
    def __len__(self):
        return len(self.buffer)

class QNetwork(nn.Module):
    def __init__(self, state_dim=6, action_dim=4, hidden_dim=64):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(state_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, action_dim),
        )
    
    def forward(self, state):
        return self.net(state)