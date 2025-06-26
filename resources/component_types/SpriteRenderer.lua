SpriteRenderer = {
	sprite = "???",
	r = 255,
	g = 255,
	b = 255,
	a = 255,
	sorting_order = 0,

	OnStart = function(self)
		self.transform = self.actor:GetComponent("Transform")
		self.cjson = require("cjson")
		Network.sendPacket(self.cjson.encode({
			sprite = self.sprite,
			x = self.transform.x,
			y = self.transform.y,
			r = self.r,
			g = self.g,
			b = self.b,
			a = self.a,
			sort = self.sorting_order
		}))
	end,

	OnUpdate = function(self)
		Image.DrawUIEx(self.sprite, self.transform.x, self.transform.y, self.r, self.g, self.b, self.a, self.sorting_order)
	end
}
