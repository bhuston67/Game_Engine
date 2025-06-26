GlobalSpriteRenderer = {
	sprites = {},

	OnStart = function(self)
		Network.Subscribe("create",self,self.OnCreate)
		Network.Subscribe("move",self,self.OnMove)
        Network.Subscribe("currentClientResponse",self,self.OnCreate)
        self.cjson = require("cjson")
        Network.sendPacket(self.cjson.encode({eventType = "currentClientRequest"}))
	end,

	OnUpdate = function(self)
		for id, transform in pairs(self.sprites) do
            print(self.cjson.encode(transform))
			Image.DrawUIEx(transform.sprite, transform.x, transform.y, transform.r, transform.g, transform.b, transform.a, transform.sort)
		end
	end,

	OnCreate = function(self,event)
        print("cerated")
		self.sprites[event.clientId] = {
			sprite = event.sprite,
			x = event.x,
			y = event.y,
			r = event.r,
			g = event.g,
			b = event.b,
			a = event.a,
			sort = event.sort }
        
	end,

	OnMove = function(self,event)
        print("move")
		self.sprites[event.clientId] = {
			sprite = event.sprite,
			x = event.x,
			y = event.y,
			r = event.r,
			g = event.g,
			b = event.b,
			a = event.a,
			sort = event.sort }
	end
}