import gridworld_module
from gridworld_gym_env import GridWorldGymEnv
from gymnasium.utils.env_checker import check_env
from agent import QLearningAgent
from visualization import plot_training_metrics

# --- Hyperparameters ---
SIZE = 10
ITERATIONS = 100

DISCOUNT = 0.9
LEARNING_RATE = 0.2
EPSILON = 1.0
EPSILON_DECAY = 0.93
MIN_EPSILON = 0.01

MAX_STEPS = 1000
STEP_TIMEOUT_PUNISHMENT = -100
MAX_CONSECUTIVE = 5  # stop early after this many episodes at best_steps

PRINT_FINAL_GRID = True


def train(env, agent):
    """Run the training loop. Returns per-episode metrics and stats."""
    best_steps = float('inf')
    best_achieved = 0
    consecutive_best_steps = 0
    total_iterations = 0
    global_total_reward = 0
    global_steps = 0

    episode_steps, episode_rewards, eplsions = [], [], []

    for i in range(ITERATIONS):
        if i > 0:
            env.generateMaze()
            env.reset()
        done = False
        total_reward = 0
        steps = 0

        while not done:
            current_state = env.posToIndex(env.pos)
            action = agent.choose_action(current_state)
            
            res = env.step(action)
            done = res.Done

            if steps >= MAX_STEPS:
                res.reward = STEP_TIMEOUT_PUNISHMENT

            next_state = env.posToIndex(env.pos)
            agent.update_q_value(current_state, next_state, int(action), res.reward, done)

            total_reward += res.reward
            steps += 1

            if steps >= MAX_STEPS:
                break

        # --- Bookkeeping ---
        if steps < best_steps:
            best_steps = steps
            best_achieved = i
        consecutive_best_steps = consecutive_best_steps + 1 if steps == best_steps and steps != MAX_STEPS else 0

        agent.decay_epsilon()
        global_total_reward += total_reward
        global_steps += steps
        total_iterations += 1
        episode_steps.append(steps)
        episode_rewards.append(total_reward)
        eplsions.append(agent.epsilon)

        print(f"Episode {i}: Total Steps = {steps}, Reward Gained = {total_reward}")

        if consecutive_best_steps >= MAX_CONSECUTIVE:
            break

    print(f"After {total_iterations} Episodes: Total Steps = {global_steps}, "
          f"Reward Gained = {global_total_reward}, Best Steps = {best_steps}")

    stats = {
        "best_steps": best_steps,
        "best_achieved": best_achieved,
        "total_iterations": total_iterations,
        "global_steps": global_steps,
        "global_total_reward": global_total_reward,
    }
    return episode_steps, episode_rewards, eplsions, stats


def run_greedy_demo(env, agent, best_steps):
    """Follow the greedy policy after training."""
    done = False
    env.reset()
    while not done:
        current_state = env.posToIndex(env.pos)
        action = agent.best_action(current_state)
        res = env.step(action)
        if best_steps < 20:
            env.printGrid()
        done = res.Done
    


def main():
    env = gridworld_module.Gridworld(SIZE)
    env.generateMaze()
    env.setGoal(67)
    env.printGrid()
    path = gridworld_module.solveAStar(env.Grid, SIZE, env.posToIndex(env.pos), env.goal)
    
    agent = QLearningAgent(
        grid_size=SIZE,
        discount=DISCOUNT,
        learning_rate=LEARNING_RATE,
        epsilon=EPSILON,
        epsilon_decay=EPSILON_DECAY,
        min_epsilon=MIN_EPSILON,
    )

    episode_steps, episode_rewards, eplisons, stats = train(env, agent)

    plot_training_metrics(episode_steps, episode_rewards, eplisons)
    
    A_starsteps = len(path) - 1

    #run_greedy_demo(env, agent, stats["best_steps"])
    print(f"Calculated A* path: {path}")
    print(f"Total A* steps: {A_starsteps}")
    print(f"Efficency: {(A_starsteps / stats['best_steps']) * 100}%")


if __name__ == "__main__":
    gym = GridWorldGymEnv(size=100)
    check_env(gym)
    
    