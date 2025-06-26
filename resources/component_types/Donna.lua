Donna = {
	lives = 3,
	iframe_cooldown = 0,
	id = 0,

	OnStart = function(self)
		self.spriterenderer = self.actor:GetComponent("SpriteRenderer")
		self.transform = self.actor:GetComponent("Transform")

		self.cjson = require("cjson")
	end,

	OnUpdate = function(self) 
		-- Network.sendPacket(self.cjson.encode({x = 5, y = 2}))
		-- -- projectile spawning
		-- if Input.GetControllerButton("A", self.id) and Application.GetFrame() % 10 == 0 then
		-- 	new_projectile = Actor.Instantiate("Projectile")
		-- 	new_projectile_transform = new_projectile:GetComponent("Transform")
		-- 	new_projectile_transform.x = self.transform.x
		-- 


	end,
}

