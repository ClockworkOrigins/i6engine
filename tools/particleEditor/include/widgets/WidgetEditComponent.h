#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__

#include <QGraphicsWidget>

namespace ParticleUniverse {
	class IElement;
} /* namespace IElement */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetEditComponent : public QGraphicsWidget {
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

	public:
		static QString EC_SYSTEM;
		static QString EC_RENDERER;
		static QString EC_TECHNIQUE;
		static QString EC_EMITTER;
		static QString EC_AFFECTOR;
		static QString EC_OBSERVER;
		static QString EC_HANDLER;
		static QString EC_BEHAVIOUR;
		static QString EC_EXTERN;
		static QString CST_UNDEFINED;

		/**
		 * \brief constructor
		 */
		WidgetEditComponent(QGraphicsScene * scene, QString name, QString type, QString subType);

		/**
		 * \brief destructor
		 */
		~WidgetEditComponent();

		QString getType() const {
			return _type;
		}

		void setPUElement(ParticleUniverse::IElement * element) {
			_element = element;
		}

	private:
		QString _type;
		ParticleUniverse::IElement * _element;

		/*QRectF boundingRect() const override;

		void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) override;*/
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__ */
