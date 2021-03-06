ig.module ('game.entities.player')
.requires ('game.warplib', 'impact.entity', 'game.entities.fighter',
 'game.entities.smallLaser', 'impact.sound', 'game.entities.player_missile', 'game.entities.laser')
.defines (function()
{
	PlayerShip = ig.Entity.extend(
	{
		armor: 10000,
		shield: 100.0,
		shieldStrengthBase: 10,
		shieldStrength: 10,
		
		energy: {s: 33, e: 33, w: 33},
		
		customMove: {
			acc: {x:0.0, y:0.0},
			vel: {x:0.0, y:0.0},
			pos: {x:0.0, y:0.0},
		},
		playerAccel: 200,
		
		fighterCount: 20,
		fighterRepair: 1,
		fighterQueue: 0,
		fighterWait: 1,
		fighterRecall: false,
		fighterArmor: 100,
		fighterDamage: 5,
		maxFighters: 10,
		fightersOut: 0,
		
		name: "PLAYER",
		materialCount: 0,
		size: {x: 56, y: 56},
		offset: {x: 2, y: -2},
		type: ig.Entity.TYPE.A,
		checkAgainst: ig.Entity.TYPE.A,
		angle: 0.0,
		game: {},
		radius: 40,
		pathRadius: 60,
		moveRadius: 200,
		zIndex: 3000,
		warpDrive: null,
		isTakingDamage: 0.0,
		friendly: true,
		topSpeed: 200,
		
		target: {},
		targetTime: 1.0,
		
		weapons: [
			{damage: 200, range: 400, reload: 0.6, delay: 0.15, fire: 1.0, charge: 0.0, target: {x: 0, y: 0}},
			{damage: 50, range: 200, reload: 0.1, delay: 0.001, fire: 1.0, charge: 0.0, target: {x: 0, y: 0}},
			{damage: 200, delay: 1.000, fire: 1.0, charge: 0.0, targetSpeed: 2}
		],
				  
		piercing: false,
				  
		thrustersGoing: false,
		thrustersSFX: new ig.Sound('media/sounds/thrusters.*'),
		missileSFX: new ig.Sound('media/sounds/missileLaunch.*'),
		fighterSFX: new ig.Sound('media/sounds/fighterLaunch.*'),
		pickupSFX: new ig.Sound('media/sounds/pickup.*'),
		thrusterTimer: new ig.Timer(),

		
		laserSFX: new ig.Sound('media/sounds/laser.*'),
		smallLaserSFX: new ig.Sound('media/sounds/smallLaser.*'),
		
		animSheet: new ig.AnimationSheet ('media/player.png', 60, 54),
		//base: new ig.AnimationSheet ('media/playershipshort.png', 60, 54),
		//baseThrusters: new ig.AnimationSheet ('media/playershipshortthrusters.png', 60, 54),
		//thrustersMode: new ig.AnimationSheet ('media/playershipthrustersmode.png', 60, 54),
		//shieldMode: new ig.AnimationSheet ('media/playershipshieldsmode.png', 60, 54),
		//shieldThrusters: new ig.AnimationSheet ('media/playershipthrustersshieldsmode.png', 60, 54),
		//weaponsMode: new ig.AnimationSheet ('media/playershipweaponsmode.png', 60, 54),
		//weaponsThrusters: new ig.AnimationSheet ('media/playershipthrustersweaponsmode.png', 60, 54),
		
		init: function (x, y, settings) {
			this.addAnim ('base', 0.1, [0], true);
			this.addAnim ('thrusters', 0.1, [2], true);
			this.addAnim ('thrustersmode', 0.1, [4], true);
			this.addAnim ('shields', 0.1, [6], true);
			this.addAnim ('shieldsthrusters', 0.1, [8], true);
			this.addAnim ('weapons', 0.1, [10], true);
			this.addAnim ('weaponsthrusters', 0.1, [12], true);
			
			this.game = settings.game;
			
			this.customMove.pos.x = x;
			this.customMove.pos.y = y;

			

			this.warpDrive = settings.state.warpDrive.stat;
			this.armor = settings.state.armor.stat;
			this.materialCount = settings.state.materials.stat;
			this.fighterCount = settings.state.fighterCount.stat;
			this.weapons[2].ammo = settings.state.missileCount.stat;
			this.weaponUpgrades(settings);
			
			this.parent (x, y, settings);
		},
		
		move: function () {
			var angle = this.angle;
			var move = this.customMove;
			if (ig.input.state ("forward")) {
				move.acc.x = Math.ceil(Math.cos(angle * Math.PI / 180) * (this.playerAccel + this.playerAccel * (this.energy.e - 33) * .03));
				move.acc.y = Math.ceil(Math.sin(angle * Math.PI / 180) * (this.playerAccel + this.playerAccel * (this.energy.e - 33) * .03));
			}
			else if (ig.input.state ("backward")) {
				move.acc.x = Math.floor(-Math.cos(angle * Math.PI / 180) * (this.playerAccel + this.playerAccel * (this.energy.e - 33) * .03));
				move.acc.y = Math.floor(-Math.sin(angle * Math.PI / 180) * (this.playerAccel + this.playerAccel * (this.energy.e - 33) * .03));
			}
			else {
				move.acc.x = 0.0;
				move.acc.y = 0.0;
			}
			if (ig.input.state ("left")) {
				this.angle -= 2;
			}
			if (ig.input.state ("right")) {
				this.angle += 2;
			}
			
			if (ig.input.state ("right") ||
			   ig.input.state ("left") ||
			   ig.input.state ("forward") ||
			   ig.input.state ("backward")) {
				if(this.energy.e === 60) {
					this.currentAnim = this.anims.thrustersmode;
				} else if (this.energy.s === 60) {
					this.currentAnim = this.anims.shieldsthrusters;
				} else if (this.energy.w === 60) {
					this.currentAnim = this.anims.weaponsthrusters;
				} else {
					this.currentAnim = this.anims.thrusters;
				}
			} else {
				if (this.energy.s === 60) {
					this.currentAnim = this.anims.shields;
				} else if (this.energy.w === 60) {
					this.currentAnim = this.anims.weapons;
				} else {
					this.currentAnim = this.anims.base;
				}
			}
			this.currentAnim
			if ((ig.input.state ("right") ||
			   ig.input.state ("left") ||
			   ig.input.state ("forward") ||
			   ig.input.state ("backward")) &&
			   !this.thrustersGoing) {
				this.thrustersSFX.play();
				this.thrustersGoing = true;
				this.thrusterTimer.set(250);
			}
			if(this.thrustersGoing &&
			   this.thrusterTimer.delta() >= 0){
				this.thrustersSFX.stop();
				this.thrustersSFX.play();
				this.thrusterTimer.set(250);
			}
			if (!ig.input.state ("right") &&
			   !ig.input.state ("left") &&
			   !ig.input.state ("forward") &&
			   !ig.input.state ("backward") &&
			   this.thrustersGoing) {
				this.thrustersSFX.stop();
				this.thrustersGoing = false;
			}
			this.currentAnim.angle = this.angle * Math.PI / 180;
		},
		
		slowDown2: function (move, corr) {
			if (move.acc[corr] < 1000 && move.vel[corr] > 100.0 / FPS) {
				move.vel[corr] -= 100.0 / FPS;
			} 
			else if (move.acc[corr] < 1000 && move.vel[corr] > 0) {
				move.vel[corr] = 0.0;
			}
			
			if (move.acc[corr] > -1000 && move.vel[corr] < -100.0 / FPS) {
				move.vel[corr] += 100.0 / FPS;
			} 
			else if (move.acc[corr] > -1000 && move.vel[corr] < 0) {
				move.vel[corr] = 0.0;
			} 
		},
		
		movement: function () {
			var move = this.customMove;
			
			if (inRange (move.vel.x + move.acc.x / FPS, 0, move.vel.y + move.acc.y / FPS, 0, this.topSpeed)) {
				move.vel.x += move.acc.x / FPS;
				move.vel.y += move.acc.y / FPS;
			}
			
			move.pos.x += move.vel.x / FPS;
			move.pos.y += move.vel.y / FPS;
			this.slowDown2 (move, 'x');
			this.slowDown2 (move, 'y');
			
			this.pos.x = move.pos.x;
			this.pos.y = move.pos.y;
		},
		
		setScreen: function () {
			this.game.screen.x = this.pos.x - 410;
			this.game.screen.y = this.pos.y - 303;
		},
		
		firePrimary: function (self, xTar, yTar, damage) {
							
			var settings = {parentShip:self, damage: damage, target: {x: xTar, y: yTar}, friendly: self.friendly,
							game: self.game, color: playerLaser, laserSFX: self.laserSFX, piercing: self.piercing};
			ig.game.spawnEntity (SimpleLaser, self.pos.x + (self.size.x/2), self.pos.y + (self.size.y/2), settings);
		},
		
		fireSecondary: function (self, xTar, yTar, damage) {
			if (!ig.input.state("firePrimary")){
				var settings ={parentShip: self, damage: damage, target: {x: xTar, y: yTar}, friendly: self.friendly,
							game:self.game, color: "green", smallLaserSFX: self.smallLaserSFX};
			
				ig.game.spawnEntity (SmallLaser, self.pos.x + (self.size.x/2), self.pos.y + (self.size.y/2), settings);
			}
		},

		fireMissile: function (player, entities){
			if(!ig.input.state ("fireMissile") || this.weapons[2].charge > 0 || this.weapons[2].ammo <= 0){
				this.weapons[2].charge -= 1.0 / FPS;
				return false;
			}
			var spawnX = (player.pos.x + Math.floor(player.size.x/2));
			var spawnY = (player.pos.y + Math.floor(player.size.y/2));
			console.log("FIRE1");
			
			if (this.targetTime <= 0) {
				var settings = {damage: player.weapons[2].damage, target: this.target, game: player.game};
				player.weapons[2].ammo--;
				this.missileSFX.play();
				ig.game.spawnEntity (playerMissile, spawnX, spawnY, settings);
				player.weapons[2].charge = player.weapons[2].delay;
			}
					
		},
		
		fire: function (weapon, action, fireWeapon) {
			var xTar = ig.input.mouse.x + this.game.screen.x;
			var yTar = ig.input.mouse.y + this.game.screen.y;
			if (ig.input.state (action) && this.weapons[weapon].charge <= 0 ) {
				
				//if (inRange(xTar, this.pos.x, yTar, this.pos.y, this.weapons[weapon].range)) {
					this.weapons[weapon].fire = this.weapons[weapon].delay;
					this.weapons[weapon].charge = this.weapons[weapon].reload;
					//this.weapons[weapon].target.x = this.pos.y + this.radius + Math.sin(angle) * this.weapons[weapon].range;
					//this.weapons[weapon].target.y = this.pos.x + this.radius + Math.cos(angle) * this.weapons[weapon].range;
				//}
			}
			
			if (this.weapons[weapon].fire <= 0) {
				var angle = Math.atan ((yTar - this.pos.y - this.radius) / (xTar - this.pos.x - this.radius));
				if (xTar - this.pos.x - this.radius < 0) {
					angle += Math.PI;
				}
				yTar = this.pos.y + this.radius + Math.sin(angle) * this.weapons[weapon].range; //this.weapons[weapon].target.x;
				xTar = this.pos.x + this.radius + Math.cos(angle) * this.weapons[weapon].range; //this.weapons[weapon].target.y;
				fireWeapon (this, xTar, yTar, this.weapons[weapon].damage +  this.weapons[weapon].damage * (this.energy.w - 33) * 0.005);
				this.weapons[weapon].fire = this.weapons[weapon].delay * 2;
			}
			
			if (this.weapons[weapon].fire < this.weapons[weapon].delay * 2) {
				this.weapons[weapon].fire -= 1.0 / FPS;
			}
			
			if (this.weapons[weapon].charge > 0) {
				this.weapons[weapon].charge -= (1.0 + (this.energy.w - 33) * 0.03) / FPS;
			}
		},
		
		fighterBay: function () {
			if (ig.input.pressed ("recall")) {
				this.fighterRecall = !this.fighterRecall;
				
			}
			if (ig.input.state ("launch") && this.fighterCount > 0 && this.fighterWait <= 0 && this.fightersOut < this.maxFighters) {
				this.fighterRecall = false;
				var settings = {game: this.game, armor: this.fighterArmor, damage: this.fighterDamage};
				if (this.targetTime < 1.0) {
					settings.target = this.target;
				}
				ig.game.spawnEntity (Fighter, this.pos.x + (this.size.x/2), this.pos.y + (this.size.y/2), settings);

				this.fighterSFX.play();			
				this.fighterCount -= 1;

				this.fightersOut++;
				this.fighterWait = 0.4;
			}
			if (this.fighterWait > 0) {
				this.fighterWait -= 1 / FPS;
			}
			if (this.fighterQueue > 0) {
				this.fighterRepair -= 1 / FPS;
			}
			if (this.fighterRepair <= 0) {
				this.fighterRepair = 1;
				this.fighterQueue -= 1;
				this.fighterCount += 1;
			}
		},
		
		targetting: function () {
			var enemies = this.game.getEntitiesByType ('Enemies');
			var tempTar = {};
			var mouseX = ig.input.mouse.x + this.game.screen.x;
			var mouseY = ig.input.mouse.y + this.game.screen.y;
			for (var i = 0; i < enemies.length; i++) {
				var ent = enemies[i];
				if (inRange(mouseX, ent.pos.x + ent.size.x/2, mouseY, ent.pos.y + ent.size.y/2, ent.radius + 30)) {
					tempTar = ent;
					if (tempTar === this.target) {
						if (this.targetTime > 0) this.targetTime -= this.weapons[2].targetSpeed / FPS;
						break;
					}
				}
			}
			if (tempTar !== this.target) {
				this.target = tempTar;
				this.targetTime = 1.0;
			}
		},
		
		draw: function () {
			this.parent();
			
			if (this.targetTime < 1 && this.target !== {}) {
				var tar = this.target;
				var tarx = tar.pos.x + tar.size.x/2 - this.game.screen.x;
				var tary = tar.pos.y + tar.size.y/2 - this.game.screen.y;
				if (this.targetTime < 0) {
					this.targetTime = 0;
				}
				var color = "#" + Math.round (this.targetTime * 9) + "0" + (9 - Math.round (this.targetTime * 9)) + "000";
				ig.system.context.beginPath();
				ig.system.context.arc(tarx, tary, tar.radius + 5, 0, 2*Math.PI - 2*Math.PI*this.targetTime);
				ig.system.context.strokeStyle = color;
				ig.system.context.lineWidth = 2;
				ig.system.context.stroke();
				ig.system.context.closePath();
			}
		},

		weaponUpgrades: function (settings) {
			var upgrades = settings.state.upgrades;

			this.weapons[0].damage = 150 + (upgrades.weapons.main.damage.stat * 100);
			this.weapons[0].reload = 1.5 - (upgrades.weapons.main.fireRate.stat * 0.1);
			this.weapons[0].delay = 0.2 - (upgrades.weapons.main.delay.stat * 0.02);
			if (upgrades.weapons.main.piercing.stat) {
				this.piercing = true;
			}
			
			this.weapons[1].damage = 50 + (upgrades.weapons.secondary.damage.stat * 25);
			this.weapons[1].reload = 0.15 - (upgrades.weapons.secondary.fireRate.stat * 0.01);
			this.weapons[1].range = 250 + (upgrades.weapons.secondary.range.stat * 100);

			this.weapons[2].damage = 500 + (upgrades.missile.damage.stat * 400);
			this.weapons[2].delay = 1.500 - (upgrades.missile.fireRate.stat * 0.1);
			this.weapons[2].targetSpeed = 3 + (upgrades.missile.targetSpeed.stat * 3);

			this.fighterArmor = 500 + (upgrades.fighters.armor.stat * 200);
			this.fighterDamage = 20 + (upgrades.fighters.damage.stat * 15);
			this.maxFighters = 10 + (upgrades.fighters.count.stat * 10);

			this.shieldStrength = 10 + (upgrades.shields.stat * 8);
            this.shieldStrengthBase = this.shieldStrength;
			this.maxArmor = 10000 + upgrades.armor.stat * 2000;
			this.armor = settings.state.armor.stat;
			this.playerAccel = 300 + (upgrades.engine.stat * 100);
		},
		
		manageEnergy: function () {
			if (ig.input.pressed ("balence")) {
				this.energy.e = 33;
				this.energy.s = 33;
				this.energy.w = 33;
				this.topSpeed = 200;
			}
			else if (ig.input.pressed ("shields")) {
				this.energy.e = 20;
				this.energy.s = 60;
				this.energy.w = 20;
				this.topSpeed = 200;
			}
			else if (ig.input.pressed ("engines")) {
				this.energy.e = 60;
				this.energy.s = 20;
				this.energy.w = 20;
				this.topSpeed = 300;
			}
			else if (ig.input.pressed ("weapons")) {
				this.energy.e = 20;
				this.energy.s = 20;
				this.energy.w = 60;
				this.topSpeed = 200;
			}
			this.shieldStrength = this.shieldStrengthBase + this.shieldStrengthBase * (this.energy.s - 33) * .03;
		},
		
		bigVal: 2,
		
		update: function () {
			this.fire(0, "firePrimary", this.firePrimary);
			this.fire(1, "fireSecondary", this.fireSecondary);
			this.fireMissile(this, this.game.entities);
			
			this.move();
			this.movement();
			this.setScreen();
			this.fighterBay();
			this.targetting();
			this.manageEnergy();
			
			rechargeShields (this);
			if (this.energy.s > 33) {
				this.shield += 2.0 / FPS;
			}
			
			this.parent();
		},
		
		check: function ( other ) {
			if(other.name === "MATERIALS"){
				this.materialCount += other.value;
				other.kill();
				this.pickupSFX.play();
			} else
			if(other.name === "ENEMY"){
				entHit(this, other.collisionDmg);
				other.explodeNoDrop();
			} else
			if(other.name === "ASTEROID"){
				entHit(this, other.damage);
				other.kill();
			}
		},

		explode: function () {
			this.kill();
		}
		
		
	});
	

});
