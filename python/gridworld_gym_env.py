import gymnasium as gym
import numpy as np
import gridworld_module
from gymnasium import spaces

class GridWorldGymEnv(gym.Env):
    """A gridworld environment."""
    def __init__(self, size=10, seed=None):
        super().__init__()
        if seed is not None:
            self.env = gridworld_module.Gridworld(size, seed)
        else:
            self.env = gridworld_module.Gridworld(size)
        
        self.size = size
        
        self.action_space = spaces.Discrete(4)
        
        low = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0], dtype=np.float32)
        high = np.array([1.0, 1.0, 1.0, 1.0, 1.0, 1.0], dtype=np.float32)
        self.observation_space = spaces.Box(low=low, high=high,dtype=np.float32)
    
    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        if seed is not None:
            self.env.seed(seed)
        
        self.env.reset()
        
        obs = np.array(self.env.getLocalObservation(), dtype=np.float32)
        info = {}
        
        return obs, info
    
    def step(self, action):
        pyaction = gridworld_module.Action(action)
        
        res = self.env.step(pyaction)
        
        obs = np.array(self.env.getLocalObservation(), dtype=np.float32)
        
        reward = res.reward
        Terminated = res.Done
        truncated = False
        info = {}
        
        return obs, reward, Terminated, truncated, info
        