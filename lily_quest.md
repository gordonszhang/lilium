# Lily Quest
## Vitals
* Top-down action game (rough ¾ths perspective)
* Boss rush, no filler levels
* Twitchy combat (e.g. parries) married with puzzler segments (timestop, will elaborate)

## Technical Things
Resolution: 320 x 180 

>Tenative (as this is quite small), but 16:9 pixel-friendly resolution is ideal. At 6x, this scales to 1920 x 1080.

FPS: 60

>Logic ticks may obviously be slower.

Animation speeds: Variable

>Need to experiment to settle on a universal standard. Flexibility would be greatly appreciated.

## Engine Overview
The engine should be built to handle various game states. Major states are listed below for an overivew of scope.

### World
Player input accepted (only movement and basic interaction). Player may interact with objects (by speaking to them). It's worth noting that enemies do not exist outside the scope of a battle.

### Pause
Menu state, accessible at any point in the game outside of cutscenes. This acts as a pause and a pause only, as there are no "items" or manual save options in this game. 

### Cutscenes
Note that this is not a separate "novel" state, but simply a state in which the game takes over control of the engine and performs actions according to a script. This might include:
 
 * Moving a character or playing an animation
 * Displaying dialogue
 * Creating particle affects

Further discussion should be had regarding the necessity of a visual novel style textbox for dialogue.

> Point: A visual novel style textbox allows for portraits to be prominently displayed.

> Counterpoint: Hand-drawn portraits are difficult to translate to a low-resolution pixel environment. A more suitable solution would be hovering speech bubbles over characters' heads.

### Battle
The meat of the game. Movement and combat inputs accepted, and enemy interactions happen here. 

## State Machine
Assuming that we are in the Battle state, a basic rundown of the game's state machine follows. **Bold** entries in the Input column indicate direct inputs from the player.

| Inital State  | Input           | Target State   |
| ------------- |-----------------|----------------|
| Idle          | **Attack**      | Attack         |
| Attack        | None            | Idle           |
| Attack        | Attack Connects with Projectile | BulletTimeProjectile*     |
| Attack        | Attack Connects with Melee | BulletTimeMelee*     |
| BulletTimeProjectile    | None            | Idle           |
| BulletTimeProjectile    | **Attack**          | Idle           |
| BulletTimeMelee    | None            | Idle           |
| BulletTimeMelee    | **Attack**          | BulletTimeMelee           |
| Attack        | **Hold Attack** | Charge         |
| Charge        | **Release Attack**| TeleportAttack |
| Charge        | **Cancel**| Idle |
| TeleportAttack| None            | Idle           |
| Any           | Take Damage     | Recoil         |
| Recoil        | None            | Idle           |

### Idle 
Just idlin'. Movement allowed.

### Attack
Getting a little aggresive, aren't we? Movement still allowed! Puts out a hitbox, which upon collision places the player into one of two special states:
* BulletTimeProjectile: Clashed with a projectile. Some bosses' dive attacks are considered projectiles...
* BulletTimeMelee: Clashed with a melee attack.

### BulletTimeProjectile
Overall game time slows to 1/10 or similar (to allow leading of targets). Player becomes invulnerble for the duration of this state, but cannot move. Projectile that player is in contact with is likewise frozen.

Player is granted an aiming reticle, and inputting another attack will deflect the projectile in the direction aimed at. The edge case where the player holds the attack button into this state can be handled by polling for an attack **release**. 

State duration is denoted by a highlighted circle that sweeps clockwise until it disappears (when the state ends).

### BulletTimeMelee
Overall game time slows, except for player. Player can freely move and attack until duration runs out.

### Charge
Windup state for TeleportAttack. Obtained by Attacking and holding the button. A growing arrow reticle denotes the range of the teleport, which increases as the button is held. Distance caps at some point. Movement allowed during this state.

Can be canceled manually without teleport by using the cancel button.

### TeleportAttack
Spin attack + teleport. Player is invulnerable for the first few frames of this, but move is laggy with a relatively long animation. Cannot move during this state. No cooldown.

### Recoil
Taking damage. Player is knocked back and rendered invulnerable for a short period. 

### Invulnerability
Likely should be handled separately from the state machine.

## Control Scheme
### Mouse and Keyboard
* Movement: WASD
* Attack: LMB
* Aiming for BulletTime and TeleportAttack: Mouse Position
* Cancel Charge: RMB
### Controller
* Movement: Left Joystick
* Attack: Square (or Y on XBOX)
* Aiming for BulletTime and TeleportAttack: Right Joystick
* Cancel Charge: Right Trigger 1 or 2

## Misc.
### Boss Ideas
#### Hunter
* Shoots, then runs to a new location for cover.

#### Witch
* Light strike array, Overhead beam that charges up and track the player until the last moment. Player must wait until the opportune moment to use their teleport, while avoiding volleys of undeflectable beams!

#### Merc
* Dangerous, sweeping sword attacks.
* Fast, charging attack, becomes a deflectable projectile. Into the wall you go!

#### Goddess
* 7 floating light-spears. Sends them at player in pairs, one after the other. Player needs to parry the first sword and deflect it into the second to avoid damage. Last odd spear is a freebie for damage.
* Dead Man's Volley
