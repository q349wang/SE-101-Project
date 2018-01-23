import java.awt.Color;
import java.awt.Graphics;

public class Collectible extends Entity{
	
	private Player player;

	public Collectible(int x, int y, int lives, int health, int size, Player player) {
		super(x, y, lives, health, size,EntityType.COLLECT, "res/fruit.png");
		this.x = x;
		this.y = y;
		this.player = player;
	}

	@Override
	public void move(int x, int y) {
		this.x +=x;
		this.y += y;
	}
	
	@Override
	public void collision(){
		if(this.collides(player)){
			this.player.setScore(this.player.getScore() + 1);
			this.isDead = true;
		}
	}
	
	@Override
	public void draw(Graphics g) {
		g.setColor(Color.GREEN);
		sprite.paintComponent(g, this.x, this.y);
	}

}
