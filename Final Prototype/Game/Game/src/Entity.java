import java.awt.Graphics;
import java.awt.Rectangle;

//constructor abstract class cannot be intialiszed
//general framework
//entity - general class for all three for them - superclass player enemy and collectible
public abstract class Entity extends Rectangle{
	
	protected int lives,health, size;
	protected boolean isDead;
	protected EntityType t;
	protected Sprite sprite;
	
	public int getHealth() {
		return health;
	}
	

	public EntityType getT() {
		return t;
	}

	public Entity(int x, int y, int lives, int health, int size, EntityType t, String filePath){
		isDead = false;
		sprite = new Sprite(filePath,size,size);
		this.setBounds(x, y, size, size);
		this.lives = lives;
		this.size = size;
		this.health = health;
		this.t =t;
	}
	//override these methods =subclasses
	public abstract void move(int x, int y);
	public abstract void draw(Graphics g);
	public abstract void collision();
	//chekc for collision
	public boolean collides(Entity a){
		if(this.intersects(a))
			return true;
		return false;
	}
}
